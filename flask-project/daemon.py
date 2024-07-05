import subprocess, os, time
os.chdir(os.path.dirname(os.path.abspath(__file__)))

def update():
    # 返回上一层目录
    os.chdir("..")
    os.system("git pull")
    os.chdir("flask-project")

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
