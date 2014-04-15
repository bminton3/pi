from flask import render_template, flash, redirect, session, url_for, request, g


@app.route('/')
def index():
        return render_template("index.html")

@app.route('/_test_button_press')
def test_button_press():
        a = request.args.get('a', 0, type=int)
        return jsonify(result = a + 1)

