import subprocess, os, time

def update():
    os.system("git pull")

if __name__ == '__main__':
    try:
        with open("UPDATE", "r") as f:
            update()
        os.remove("UPDATE")
    except FileNotFoundError:
        pass

    while(1):
        subprocess.run(["python", "app.py"])
        try:
            with open("UPDATE", "r") as f:
                update()
                time.sleep(0.5)
            # Remove the file "UPDATE"
            os.remove("UPDATE")
        except FileNotFoundError:
            time.sleep(5)
