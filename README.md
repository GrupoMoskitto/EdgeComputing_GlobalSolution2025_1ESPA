# EdgeComputing_GlobalSolution2025_1ESPA
imperial circus dead decadence - goku (official music video)
codigo pra requisicao:

python (http.client):

```
import http.client

conn = http.client.HTTPConnection("20.55.19.44", 1026)
payload = ''
headers = {
  'Accept': 'application/json',
  'fiware-service': 'smart',
  'fiware-servicepath': '/'
}
conn.request("GET", "/v2/entities/urn:ngsi-ld:SensorDevice:001?type=SensorDevice", payload, headers)
res = conn.getresponse()
data = res.read()
print(data.decode("utf-8"))
```

python (requests):

```
import requests

url = "http://20.55.19.44:1026/v2/entities/urn:ngsi-ld:SensorDevice:001?type=SensorDevice"

payload = {}
headers = {
  'Accept': 'application/json',
  'fiware-service': 'smart',
  'fiware-servicepath': '/'
}

response = requests.request("GET", url, headers=headers, data=payload)

print(response.text)

```

curl:

```
curl --location 'http://20.55.19.44:1026/v2/entities/urn:ngsi-ld:SensorDevice:001?type=SensorDevice' \
--header 'Accept: application/json' \
--header 'fiware-service: smart' \
--header 'fiware-servicepath: /'
```

postman (exportado):

```nao sei como exportar xd```
