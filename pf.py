#!/usr/bin/env python3

import time
from multiprocessing import Pool
from PIL import Image
from socket import socket


#IMAGE_FILE = "sea-watch_logo_140.png"
IMAGE_FILE = "python-powered-w-200x80.png"
COLOR_IGNORE = (255, 255, 255)
OFFSET_X = 770
OFFSET_Y = 400
PROCESSES = 10

TARGET_HOST = "flood.schenklflut.de"
TARGET_PORT = 1234


def main():
    im = Image.open(IMAGE_FILE)
    im = im.convert("RGBA")

    commands = [
        command(x + OFFSET_X, y + OFFSET_Y, im.getpixel((x, y)))
        for x in range(im.width)
        for y in range(im.height)
        if im.getpixel((x, y))[3] == 255  # and im.getpixel((x, y))[:3] != COLOR_IGNORE
    ]

    for i in range(PROCESSES):
        with open(filename(i), "w") as f:
            f.writelines(commands)

    with Pool(processes=PROCESSES) as p:
        results = [p.apply_async(send, [TARGET_HOST, TARGET_PORT, filename(i)]) for i in range(PROCESSES)]
        while not all(result.ready() for result in results):
            time.sleep(PROCESSES)

        p.join()


def filename(i: int) -> str:
    return f"{IMAGE_FILE}_{i:02d}.pf"


def command(x: int, y: int, rgba: tuple[int, int, int, int]) -> str:
    r, g, b, _ = rgba
    return f"PX {x} {y} {r:02x}{g:02x}{b:02x}\n"


def send(host: str, port: int, filename: str):
    sock = socket()
    sock.connect((host, port))

    with open(filename, "rb") as f:
        while(True):
            sock.sendfile(f)


if __name__ == "__main__":
    main()
