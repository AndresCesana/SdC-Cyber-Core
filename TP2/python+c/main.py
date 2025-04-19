import requests
import ctypes
from ctypes import c_float, c_int, POINTER

# Load the C shared library
# The shared library `libaverage.so` contains the C function `process_array`
lib = ctypes.CDLL('./libaverage.so')

# Define the argument types and return type for the C function `process_array`
# - The function expects a pointer to a float array and an integer (array size)
# - The function does not return any value
lib.process_array.argtypes = [POINTER(c_float), c_int]
lib.process_array.restype = None

# Prompt the user to enter a country code (e.g., AR for Argentina)
country_code = input("Enter the country code (e.g., AR for Argentina): ").upper()

# Construct the API URL to fetch GINI index data for the given country
# The API fetches data from 2011 to 2022 in JSON format
url = f"https://api.worldbank.org/v2/en/country/{country_code}/indicator/SI.POV.GINI?format=json&date=2011:2022"

# Send a GET request to the World Bank API
response = requests.get(url)

# Check if the API request was successful
if response.status_code == 200:
    # Parse the JSON response
    data = response.json()

    # Extract the GINI index values from the response
    # - Only include entries where the 'value' field exists and is not None
    raw_values = [entry['value'] for entry in data[1] if 'value' in entry and entry['value'] is not None]

    # Print the extracted GINI index values
    print(f"\nFloat array from API (GINI index): {raw_values}")

    # Calculate and print the average GINI index over the specified years
    average = sum(raw_values) / len(raw_values)
    print(f"Average GINI index from 2011 to 2022: {average:.2f}")

    # Convert the Python list of floats (`raw_values`) into a C-compatible float array
    # - `c_float * len(raw_values)` creates a C array type with the same size as `raw_values`
    # - `array_type(*raw_values)` initializes the C array with the values from `raw_values`
    array_type = c_float * len(raw_values)
    c_array = array_type(*raw_values)

    # Call the C function `process_array` to process the array
    # - Pass the C array and its length to the function
    print("\n--- Processing in C ---")
    lib.process_array(c_array, len(raw_values))
else:
    # Print an error message if the API request failed
    print(f"Failed to fetch data for country code {country_code}. Status code: {response.status_code}")
