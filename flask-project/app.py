from flask import Flask, redirect, request
from threading import Thread
from werkzeug.serving import make_server
import time

PORT = 1001
TOKEN = "123456"

app = Flask(__name__, static_folder='../vue-project/dist', static_url_path='/')
server = make_server('0.0.0.0', PORT, app)

def _update():
    open("UPDATE", "w")
    time.sleep(1)
    server.shutdown()

@app.route('/')
def index():
    return redirect('/index.html')

@app.route('/update')
def update():
    if request.args.get("token") != TOKEN:
        return "0"
    Thread(target=_update).start()
    return "1"

if __name__ == '__main__':
    thread = Thread(target=server.serve_forever, daemon=True)
    thread.start()
    thread.join()
