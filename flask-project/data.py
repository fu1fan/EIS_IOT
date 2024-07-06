import os
import time
import json
import requests

API_PORT = 1001

class Data:
    def __init__(self):
        try:
            with open("data.json", "r") as f:
                self.data = json.load(f)
        except FileNotFoundError:
            self.data = {}

