from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

@app.route('/')
def index():
        return render_template("index.html")

@app.route('/_test_button_press')
def test_button_press():
        a = request.args.get('a', 0, type=int)
        return jsonify(result = a + 1)


if __name__ == '__main__':
	app.run( host="0.0.0.0", port=int("5000"), debug=True)

import views
