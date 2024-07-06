from math import atan
from flask import Flask, redirect, request
from threading import Thread
from werkzeug.serving import make_server
import time
from queue import Queue
import os

PORT = 1001

with open("SYNC_TOKEN", "r") as f:
    TOKEN = f.read().strip()

os.chdir(os.path.dirname(os.path.abspath(__file__)))

app = Flask(__name__, static_folder='../', static_url_path='/')
server = make_server('0.0.0.0', PORT, app)

battery_count = -1
last_h_query = 0

shutdown = False

class ResultA:
    mode = 0
    voltages = []
    impedances = []

class ResultS:
    cell_id = 0
    freq = 0
    voltage = 0
    imag = 0.0
    real = 0.0
    zabs = 0.0
    zarg = 0.0

class Task:
    task_id = 0
    status_code = 0x00
    task_mode = None # "all" or "single"
    result = None

task_queue = Queue()
running_task = None
results = {}

def exist_task(task_id):
    for task in task_queue.queue:
        if task.task_id == task_id:
            return True
    return False

def _update():
    global shutdown
    open("UPDATE", "w")
    time.sleep(1)
    print("shutting down...")
    shutdown = True
    server.shutdown()

def _is_online():
    return time.time() - last_h_query < 30

@app.route('/')
def index():
    return redirect('/index.html')

@app.route('/update')
def update():
    if request.args.get("token") != TOKEN:
        return "0"
    Thread(target=_update).start()
    return "1"

@app.route('/api/')
def api():
    return "2024"

@app.route('/api/h/init', methods=['POST'])
def h_init():
    global last_h_query, running_task, battery_count
    last_h_query = time.time()
    running_task = None
    try:
        battery_count = int(request.get_data().decode("ascii"))
    except ValueError:
        return "0"
    return "1"

@app.route('/api/h/get_task')
def h_get_task():
    global running_task, last_h_query
    last_h_query = time.time()
    if running_task:
        running_task = None
    if task_queue.empty():
        return "null"
    task = task_queue.get()
    message = str(task.task_id) + "|" + task.task_mode
    if task.task_mode == "single":
        message += "|" + str(task.result.cell_id) + "|" + str(task.result.freq) + "|"
    elif task.task_mode == "all":
        message += "|" + str(task.result.mode) + "|"
    running_task = task
    return message

@app.route('/api/h/submit_result', methods=['POST'])
def h_submit_result():
    global results
    data = request.get_data().decode("ascii").split("|")

    try:
        task_id = int(data[0])
    except ValueError:
        return "0"
    
    if task_id != running_task.task_id:
        return "0"
    
    task = running_task
    
    try:
        if data[1] == "failed":
            task.status_code = int(data[2])
        elif data[1] == "single":
            task.result.freq = int(data[2])
            task.result.voltage = float(data[3])
            task.result.real = float(data[4])
            task.result.imag = float(data[5])
            task.result.zabs = (task.result.imag ** 2 + task.result.real ** 2) ** 0.5
            task.result.zarg = atan(task.result.imag / task.result.real)
        elif data[1] == "all":
            task.result.voltages = [float(x) for x in data[2].split(",")]
            if task.result.mode != 1:
                task.result.impedances = [float(x) for x in data[3].split(",")]
    except (ValueError, IndexError):
        return "0"
    
    running_task = None
    results[task_id] = task.result

    return "1"

@app.route('/api/c/add_task', methods=['POST'])
def c_add_task():
    if not _is_online():
        return {"status": "error", "message": "offline"}
    try:
        data = request.get_json()
    except:
        return {"status": "error", "message": "Invalid JSON data"}
    task_type = data.get("type")
    if not task_type in ["all", "single"]:
        return {"status": "error", "message": "Invalid task type"}
    result = None
    if task_type == "single":
        result = ResultS()
        try:
            cell_id = int(data["cell_id"])
        except (KeyError, ValueError):
            return {"status": "error", "message": "Invalid cell ID"}
        result.cell_id = cell_id
        try:
            freq = int(data["freq"])
        except (KeyError, ValueError):
            return {"status": "error", "message": "Invalid frequency"}
        result.freq = freq
    elif task_type == "all":
        result = ResultA()
        try:
            mode = int(data["mode"])
        except (KeyError, ValueError):
            return {"status": "error", "message": "Invalid mode"}
        if mode not in [0, 1, 2]:
            return {"status": "error", "message": "Invalid mode"}
        result.mode = mode
    task = Task()
    id = hash(time.time())
    while exist_task(id):
        time.sleep(0.01)
        id = hash(time.time())
    task.task_id = id
    task.task_mode = task_type
    task.result = result

    task_queue.put(task)

    return {"status": "success", "id": id}

@app.route('/api/h/cur_task')
def h_cur_task():
    global running_task
    if not _is_online():
        return {"status": "error", "message": "offline"}
    if running_task is None:
        if task_queue.empty():
            return {"status": "success", "task_id": -1}
        running_task = task_queue.get()
    return {"status": "success", "task_id": running_task.task_id}

@app.route('/api/c/is_online')
def c_is_online():
    return {"status": "success", "data": _is_online()}

@app.route('/api/c/battery_count')
def c_battery_count():
    global battery_count
    if _is_online():
        return {"status": "success", "data": battery_count}
    else:
        battery_count = -1
        return {"status": "error", "message": "offline"}

@app.route('/api/c/get_result')
def c_get_result():
    task_id = int(request.args.get("id"))
    if task_id not in results:
        for task in task_queue.queue:
            if task.task_id == task_id:
                return {"status": "warning", "message": "waiting"}
        if task_id == running_task.task_id:
            return {"status": "warning", "message": "processing"}
        return {"status": "error", "message": "notfound"}
    result = results[task_id]
    results.pop(task_id)
    return {"status": "success", "data": results}

if __name__ == '__main__':
    thread = Thread(target=server.serve_forever, daemon=True)
    thread.start()
    print("Server started.")
    try:
        while(1):
            time.sleep(1)
            if (not thread.is_alive()) or shutdown:
                print("Server stopped.")
                break
    except KeyboardInterrupt:
        server.shutdown()
        thread.join()
        print("Server stopped.")
