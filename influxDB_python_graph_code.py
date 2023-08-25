import pandas as pd
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

# Replace these variables with your InfluxDB credentials and information
url = "http://localhost:8086"
token = "zoFOSjhZn0BlBkaUCLvJuTZaRbxKUFIXRXj4rBFE62fqiTmbeooFIQYWPokqb0bes5kdRWmIgBfNPkGOsGNx5w=="
org = "MIT"  # Use "MIT" as the organization name
bucket = "npk_value"  # Use "NPK_values" as the bucket name


def main():
    # Initialize the InfluxDB client
    client = InfluxDBClient(url=url, token=token, org=org)

    # Read data from the Excel sheet
    excel_file = "C:/Users/rumaa/OneDrive/npk_test.xlsx"
    df = pd.read_excel(excel_file)

    # Convert and write data to InfluxDB
    write_data(client, bucket, df)



def write_data(client, bucket_name, data):
    print("Writing data to InfluxDB...")

    # Loop through each row of the DataFrame and write data to InfluxDB
    write_api = client.write_api(write_options=SYNCHRONOUS)
    for _, row in data.iterrows():
        measurement = "soil_data"
        tags = {
            "zone": row["Zone"],
            "moisture_level": row["Moisture Level"]
        }
        fields = {
            "N": row["N"],
            "P": row["P"],
            "K": row["K"],
            "EC": row["EC"],
            "pH": row["pH"]
        }
        # Create a new data point
        data_point = Point(measurement).time(time=None)

        # Add tags to the data point
        for tag_key, tag_value in tags.items():
            data_point = data_point.tag(tag_key, tag_value)

        # Add fields to the data point
        for field_key, field_value in fields.items():
            data_point = data_point.field(field_key, field_value)

        # Write the data point
        write_api = client.write_api(write_options=SYNCHRONOUS)
        write_api.write(bucket=bucket_name, record=data_point)

    print("Data written successfully.")


if __name__ == "__main__":
    main()