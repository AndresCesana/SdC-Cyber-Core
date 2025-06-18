import requests

# Fetch GINI data
url = f"https://api.worldbank.org/v2/en/country/AR/indicator/SI.POV.GINI?format=json&date=2011:2022"
response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    raw_values = [entry['value'] for entry in data[1] if 'value' in entry and entry['value'] is not None]

    print(f"Float array from API (GINI index): {raw_values}")
    
    # Increment each value in raw_values by 1
    raw_values_incremented = [value + 1 for value in raw_values]
    print(f"Float values incremented by 1: {raw_values_incremented}")

    # Convert array to ints
    int_values = [round(value) for value in raw_values]
    print(f"\nInteger-converted values: {int_values}")

    # Increment each value in int_values by 1
    int_values_incremented = [value + 1 for value in int_values]
    print(f"Integer values incremented by 1: {int_values_incremented}")


else:
    print(f"Failed to fetch data. HTTP Status Code: {response.status_code}")
