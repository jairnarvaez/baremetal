import time

TIMEOUT = 2.0

ERRORS = {
    "ERR 01": "unknown command or action",
    "ERR 02": "incorrect arguments",
    "ERR 03": "value out of range",
    "ERR 04": "peripheral not initialized",
    "ERR 05": "invalid pin or port",
    "ERR 06": "peripheral timeout",
}


def send(ser, cmd):
    ser.write((cmd.strip() + "\r").encode())
    time.sleep(0.05)
    raw = ser.read(ser.in_waiting).decode(errors="replace").strip()
    return raw if raw else None


def decode(response):
    if response is None:
        return "\033[31mERR\033[0m — no response from device"

    if response.startswith("OK"):
        value = response[2:].strip()
        if value:
            return f"\033[32mOK\033[0m {value}"
        return "\033[32mOK\033[0m"

    if response.startswith("ERR"):
        desc = ERRORS.get(response, "unknown error")
        return f"\033[31m{response}\033[0m — {desc}"

    return response
