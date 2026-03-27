import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import os

os.chdir(os.path.dirname(os.path.abspath(__file__)))

sender = ""
password = ""

with open('MAIL', 'r') as f:
    sender = f.readline().strip()
    password = f.readline().strip()

class Poster:
    def __init__(self, to=sender):
        self.to = to

    def send(self, title, content, max_retry=3):
        global sender, password
        msg = MIMEMultipart()
        msg['From'] = sender
        msg['To'] = self.to
        msg['Subject'] = title
        msg.attach(MIMEText(content, 'plain'))

        while max_retry > 0:
            try:
                with smtplib.SMTP("smtp.qq.com", 587) as server:
                    server.starttls()
                    server.login(sender, password)
                    server.sendmail(sender, self.to, msg.as_string())
                return True
            except Exception as e:
                print(e)
                max_retry -= 1
        if max_retry == 0:
            print("Failed to send email")
            return False

if __name__ == '__main__':
    poster = Poster()
    poster.send("Test", "邮件服务测试")
