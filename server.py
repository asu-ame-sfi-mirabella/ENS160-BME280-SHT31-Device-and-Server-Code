from flask import Flask, request, render_template_string, jsonify
import json

app = Flask(__name__)

# Global variable to store the latest data
latest_data = {}

@app.route('/', methods=['POST', 'GET'])
def receive_data():

    global latest_data
    if request.method == 'POST':
        # Parse received data
        data = request.data.decode()
        latest_data = json.loads(data)
        print("Received Data:", latest_data)
        return json.dumps({'status': 'success'}), 200

    elif request.method == 'GET':
        # Return the latest data
        return jsonify(latest_data)

@app.route('/display')
def display_data():
    # Display the data on an HTML page
    html = """
    <html>
        <head>
            <title>Sensor Data</title>
            <script>
                function fetchData() {
                    fetch('/data')
                        .then(response => response.json())
                        .then(data => {
                            let list = '';
                            for (const [key, value] of Object.entries(data)) {
                                list += '<li><strong>' + key + ':</strong> ' + value + '</li>';
                            }
                            document.getElementById('dataList').innerHTML = list;
                        })
                        .catch(error => console.error('Error:', error));
                }

                setInterval(fetchData, 2000); // Update every 2 seconds
            </script>
        </head>
        <body onload="fetchData()">
            <h1>Sensor Data</h1>
            <ul id="dataList">
                <!-- Data will be inserted here -->
            </ul>
        </body>
    </html>
    """
    return render_template_string(html)

@app.route('/data')
def send_data():
    global latest_data
    return latest_data

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=7771, debug=True)