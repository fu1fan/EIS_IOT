import requests, traceback

PORT = 1001

task1 = {
    "type": "all",
    "mode": 1,
}

task2 = {
    "type": "single",
    "cell_id": 1,
    "freq": 120,
}

def h_init():
    return requests.post(f"http://localhost:{PORT}/api/h/init", data=b"12").text

def h_get_task():
    return requests.get(f"http://localhost:{PORT}/api/h/get_task").text

def h_submit_result(task_id, result):
    return requests.post(f"http://localhost:{PORT}/api/h/submit_result", data=f"{task_id}|{result}".encode("ascii")).text

def add_task(task):
    r = requests.post(f"http://localhost:{PORT}/add_task", json=task)
    return r.json().get("id")

def get_result(id):
    return requests.get(f"http://localhost:{PORT}/result?id={id}").json()

def get_battery():
    return requests.get(f"http://localhost:{PORT}/battery_count").json()

if __name__ == "__main__":
    add_task(task1)