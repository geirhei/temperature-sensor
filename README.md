# Temperature Sensor Reader
An application that reads ADC output from a sensor periodically, converts it to a temperature in Celsius, and sends it to and HTTP REST endpoint.

You can assume that the value has been read using an ADC with 12-bit resolution, with a reference voltage of 3.3 V.
The temperature sensor reports a temperature range of -50C to +50C.
For example, the ADC can read the following values from the sensor:

- 2048 (roughly 0C)
- 3000 (roughly 23C)

For the sake of this exercise, the ADC values will be provided in a text file `temperature.txt` with a single value on every line.

## Requirements
- A new value can only be read from the sensor every 100 ms.
- The application shall calculate the max, min, and average temperature over a period of 2 minutes.
- Every 2 minutes the application shall send the stored values to an HTTP REST API with the address `http://localhost:5000/api/temperature` using an HTTP POST request.
- The values shall be sent in JSON format using the following structure denoted by TemperatureMeasurement:

```
// TemperatureMeasurement
{
	"Time": {
		"Start": string, // Start date and time in ISO8601 format for the measurement
		"End": string // End date and time in ISO8601 format for the measurement
	},
	"Min": number, // Minimum observed temperature
	"Max": number, // Maximum observed temperature
	"Average": number // Average temperature
}
```

All time values should follow the ISO 8601 standard with both date and time in UTC. All number values should be doubles with a maximum of two digits after the decimal.

## Error handling
It must be expected that the backend will fail quite often and return HTTP status code 500.
All errors that could happen on the server side should be handled by temporarily storing the last produced TemperatureMeasurement.
If a failure has occurred, the previously failed TemperatureMeasurement values should be sent on the next 2-minute interval.
Upon failure, the last maximum 10 TemperatureMeasurement values should be sent to an alternative endpoint `http://localhost:5000/api/temperature/missing` in JSON array format, e.g.:

```
[
	{
		"Time": {
			"Start": string,
			"End": string
		},
		"Min": number,
		"Max": number,
		"Average": number
	},
	{
		"Time": {
			"Start": string,
			"End": string
		},
		"Min": number,
		"Max": number,
		"Average": number
	},
	...
]
```

## Server side software
A piece of server side software is provided that is implemented using ASP.NET that can be used for testing.
The software exposes an API on the address `http://localhost:5000`.
The endpoints mentioned further up in this doc receive temperature values, perform simple validation of the input, format it, and store it in a text file `output.txt` relative to the executable.
Example `output.txt`:

```
[2021-04-12T20:20:20.0000000Z-2021-04-12T20:22:20.0000000Z] max: 1.13 min: 1.11 avg: 1.12
[2021-04-12T20:22:20.0000000Z-2021-04-12T20:24:20.0000000Z] max: 1.15 min: 1.11 avg: 1.13
[2021-04-12T20:24:20.0000000Z-2021-04-12T20:26:20.0000000Z] max: 1.14 min: 1.12 avg: 1.13
```

## Additional Content
Since the application is asynchronous and based on ASIO timers, we need a way to control the duration and expiry of these in order for the application to be testable. The test directory contains an example of how we can implement our own custom timer type, which gives us full control over time and duration.