from flask import Flask, request, redirect, url_for, render_template
app = Flask(__name__)

messages = []
messages2 = []
messages3 = []
@app.route("/" , methods = ['GET'])
def index():
    return render_template("index.html", msgs=messages, msgs2=messages2,msgs3 = messages3)
@app.route('/api/msg/dsl001', methods=['POST'])
def receive():
    print("Received a POST request!")
    messages.clear()
    data = request.get_json(force=True) #непанімаю
    print("Data:", data)  # <--- print what was received
    text = data.get('text', '')
    messages.append(text)
    return {'status': 'ok'}, 200
@app.route('/api/msg/dsl002', methods=['POST'])
def receive2():
    print("Received a POST request!")
    messages2.clear()
    data = request.get_json(force=True) #непанімаю
    print("Data:", data)  # <--- print what was received
    text = data.get('text', '')
    messages2.append(text)
    return {'status': 'ok'}, 200
@app.route('/api/msg/dsl003', methods=['POST'])
def receive3():
    print("Received a POST request!")
    messages3.clear()
    data = request.get_json(force=True) #непанімаю
    print("Data:", data)  # <--- print what was received
    text = data.get('text', '')
    messages3.append(text)
    return {'status': 'ok'}, 200
if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=5000)