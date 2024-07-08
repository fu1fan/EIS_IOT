from math import atan
from flask import Flask, redirect, request
from threading import Thread
from werkzeug.serving import make_server
import time
from queue import Queue
import os
import traceback

PORT = 1001

os.chdir(os.path.dirname(os.path.abspath(__file__)))
static_floder = "../" if os.path.isfile("../index.html") else "../vue-project/dist/"

with open("SYNC_TOKEN", "r") as f:
    TOKEN = f.read().strip()

app = Flask(__name__, static_folder=static_floder, static_url_path='/')
server = make_server('0.0.0.0', PORT, app)

battery_count = -1
last_h_query = 0
last_task = 0

shutdown = False

class ResultA:
    def __init__(self):
        self.mode = 0
        self.voltages = []
        self.impedances = []
    
    def __json__(self):
        return {
            "mode": self.mode,
            "voltages": self.voltages,
            "impedances": self.impedances
        }

class ResultS:
    def __init__(self):
        self.cell_id = 0
        self.freq = 0
        self.voltage = 0
        self.imag = 0.0
        self.real = 0.0
        self.zabs = 0.0
        self.zarg = 0.0

    def __json__(self):
        return {
            "cell_id": self.cell_id,
            "freq": self.freq,
            "voltage": self.voltage,
            "imag": self.imag,
            "real": self.real,
            "zabs": self.zabs,
            "zarg": self.zarg
        }

class ReusltE:
    def __init__(self):
        self.cell_id = 0
        self.freqs = []
        self.reals = []
        self.imags = []

    def __json__(self):
        return {
            "cell_id": self.cell_id,
            "freqs": self.freqs,
            "reals": self.reals,
            "imags": self.imags
        }


class Task:
    task_id = 0
    status_code = 0x00
    task_mode = None # "all" or "single"
    result = None

class State:
    state = "正常"
    last_h_query = 0
    ohmages = []
    # ohmages_mean = 0 计算得出
    voltages_cur = []
    voltages_his = Queue()
    # voltages_mean = 0 计算得出
    # voltages_total = 0 计算得出
    def __json__(self):
        return {
            "state": self.state,
            "battery_count": battery_count,
            "ohmages": self.ohmages,
            "ohmages_mean": sum(self.ohmages) / len(self.ohmages) if self.ohmages else 0,
            "voltages_cur": self.voltages_cur,
            "voltages_his": list(self.voltages_his.queue),
            "voltage_mean": sum(self.voltages_cur) / len(self.voltages_cur),
            "voltage_total": sum(self.voltages_cur),
            "last_update": last_task
        }

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
    global last_h_query, running_task, battery_count, last_task
    last_task = 0
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
    task = task_queue.queue[0]
    message = str(task.task_id) + "|" + task.task_mode
    if task.task_mode == "single":
        message += "|" + str(task.result.cell_id) + "|" + str(task.result.freq) + "|"
    elif task.task_mode == "all":
        message += "|" + str(task.result.mode) + "|"
    elif task.task_mode == "eis":
        message += "|" + str(task.result.cell_id) + "|"
    return message

@app.route('/api/h/confirm_task', methods=['POST'])
def h_confirm_task():
    global running_task, last_h_query
    last_h_query = time.time()
    data = request.get_data().decode("ascii")
    try:
        task_id = int(data)
    except ValueError:
        return "0"
    if task_queue.empty():
        return "0"
    if task_queue.queue[0].task_id != task_id:
        return "0"
    running_task = task_queue.get()
    return "1"

@app.route('/api/h/submit_result', methods=['POST'])
def h_submit_result():
    print(0)
    global results, running_task, last_h_query
    last_h_query = time.time()
    data = request.get_data().decode("ascii").split("|")

    try:
        task_id = int(data[0])
    except ValueError:
        return "0"
    
    if running_task is None:
        return "0"

    if task_id != running_task.task_id:
        return "0"
    
    task = running_task
    print(1)
    
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
            if task.result.mode != 0:
                task.result.impedances = [float(x) for x in data[3].split(",")]
        elif data[1] == "eis":
            print(2)
            task.result.freqs = [float(x) for x in data[2].split(",")]
            task.result.reals = [float(x) for x in data[3].split(",")]
            task.result.imags = [float(x) for x in data[4].split(",")]
            
    except (ValueError, IndexError):
        traceback.print_exc()
        print(data)
        return "0"
    
    running_task = None
    results[task_id] = task

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
    if task_type is None:
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
    elif task_type == "eis":
        result = ReusltE()
        try:
            cell_id = int(data["cell_id"])
        except (KeyError, ValueError):
            return {"status": "error", "message": "Invalid cell ID"}
        result.cell_id = cell_id
    else:
        return {"status": "error", "message": "Invalid task type"}
    task = Task()
    id = hash(time.time())
    while exist_task(id):
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
        if running_task:
            if task_id == running_task.task_id:
                return {"status": "warning", "message": "processing"}
        return {"status": "error", "message": "notfound"}
    task = results[task_id]
    results.pop(task_id)
    return {"status": "success", "code": task.status_code, "data": task.result.__json__() if task else {}}

@app.route('/api/c/get_state')
def c_get_state():
    return {"status": "success", "data": State().__json__()}

def task_setter():
    global last_task
    while(1):
        time.sleep(1)
        if _is_online():
            if time.time() - last_task > 60:
                task = Task()
                id = hash(time.time())
                while exist_task(id):
                    id = hash(time.time())
                task.task_id = id
                task.task_mode = "all"
                task.result = ResultA()
                task.result.mode = 1
                task_queue.put(task)

                cur = time.time()
                flag = False
                while(time.time() - cur < 15):
                    if task.task_id in results:
                        if task.status_code != 0:
                            break
                        results.pop(task.task_id)
                        last_task = time.time()
                        # 处理数据

                        State.voltages_cur = task.result.voltages
                        State.ohmages = task.result.impedances
                        if len(State.voltages_his.queue) > 7:
                            State.voltages_his.get()
                        State.voltages_his.put(sum(task.result.voltages))

                        # 如果电压最大差值大于0.5V，认为电池不平衡
                        if max(State.voltages_cur) - min(State.voltages_cur) > 0.5:
                            State.state = "均衡"
                        else:
                            State.state = "正常"
                        
                        # 如果有电池电压低于3.0V，认为电池电压过低
                        if min(State.voltages_cur) < 3.0:
                            State.state = "低压"
                        
                        # 如果有电池内阻大于30mΩ，认为电池内阻过高
                        if max(State.ohmages) > 30 or 0 in State.ohmages:
                            State.state = "异常"

                        flag = True
                        break
                    time.sleep(0.5)
                if not flag:
                    if _is_online():
                        State.state = "未响应"
                        if task in task_queue.queue:
                            task_queue.queue.remove(task)
                    else:
                        last_task = 0
        else:
            State.state = "离线"
            

if __name__ == '__main__':
    thread = Thread(target=server.serve_forever, daemon=True)
    thread.start()
    routine = Thread(target=task_setter, daemon=True)
    routine.start()
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
