import requests

for i in range(0, 1000):
	r = requests.get('http://localhost:8081/')
	h = requests.head('http://localhost:8082/')
	p = requests.post('http://localhost:8083/python.html', "THIS IS A BODY")
	print(i)
	
print(r.text)
