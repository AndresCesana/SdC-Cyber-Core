import requests

# Fetch GINI data
country_code = input("Enter the country code (e.g., AR for Argentina): ").upper()
url = f"https://api.worldbank.org/v2/en/country/{country_code}/indicator/SI.POV.GINI?format=json&date=2011:2022"
response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    raw_values = [entry['value'] for entry in data[1] if 'value' in entry and entry['value'] is not None]

    print(f"\nFloat array from API (GINI index): {raw_values}")
    average = sum(raw_values) / len(raw_values)
    print(f"Average GINI index from 2011 to 2022: {average:.2f}")


    # Convert array to ints
    int_values = [int(value) for value in raw_values]
    print(f"\nInt array from API (GINI index): {int_values}")

    # Calculate average of int values
    average_int = sum(int_values) / len(int_values)
    print(f"Average GINI index from 2011 to 2022 (int): {average_int:.2f}")


else:
    print(f"Failed to fetch data. HTTP Status Code: {response.status_code}")