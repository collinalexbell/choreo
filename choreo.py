import serial
import time

class Bot:
    zrot_id = 2
    gripper_id = 0
    backarm_id = 3
    forarm_id = 1
    motor_ids = [
        gripper_id,
        backarm_id,
        zrot_id,
        forarm_id
    ]
    def __init__(self, output_stream):
        self.output_stream = output_stream


    def init_movements_per_motor(movements):
        movements_per_motor = {}
        for _, movement in enumerate(movements):
            id = movement[0]
            bytes_arr = movement[1]
            if id in movements_per_motor:
                movements_per_motor[id].append(bytes_arr)
            else:
                movements_per_motor[id] = [bytes_arr]
        return movements_per_motor

    def n_bytes(integer, n):
        return integer.to_bytes(n, "big", signed=True)

    def extend_output_per_motor(output, movements_per_motor):
        for _, id in enumerate(Bot.motor_ids):
            if id in movements_per_motor:
                output.extend(Bot.n_bytes(id, 1))
                num_movements = len(movements_per_motor[id])
                output.extend(Bot.n_bytes(num_movements, 1))
                motor_movements = []
                for procedure_id, motor_movement in enumerate(movements_per_motor[id]):
                    output.extend(Bot.n_bytes(procedure_id, 1))
                    output.extend(motor_movement)

    def extend_output_with_num_motors(output, movements_per_motor):
        num_motors = len(movements_per_motor)
        output.extend(Bot.n_bytes(num_motors, 1))

    def exec_procedure(self, movements):
        output = bytearray()

        movements_per_motor = Bot.init_movements_per_motor(movements)

        Bot.extend_output_with_num_motors(output, movements_per_motor)
        Bot.extend_output_per_motor(output, movements_per_motor)

        self.output_stream.write(bytes(output))

    def move(motor_id, degrees, in_millis):
        b_degrees = Bot.n_bytes(degrees, 2)
        b_millis = Bot.n_bytes(in_millis, 2)
        return (motor_id, b_degrees + b_millis)

    def backarm(self, degrees, in_millis):
        return Bot.move(self.backarm_id, degrees, in_millis)

    def zrot(self, degrees, in_millis):
        return Bot.move(self.zrot_id, degrees, in_millis)

    def forarm(self, degrees, in_millis):
        return Bot.move(self.forarm_id, degrees, in_millis)

    def gripper(self, degrees, in_millis):
        return Bot.move(self.gripper_id, degrees, in_millis)
    
    def raise_backarm(self, degrees, in_millis):
        pass
        
    def lower_backarm(self, degrees, in_millis):
        degrees = -degrees

    def close_pincer(self, amount):
        pin = 0
        # close is in negative
        amount = -amount 


def simple_command():
    ser = serial.Serial("/dev/ttyUSB0")
    time.sleep(3)
    print("opened")
    time.sleep(3)
    ser.write(b'\x01\x01\x01\x00\x00\x28\x07\xD0')
    time.sleep(6)


def test_single_motor_movement():
    ser = serial.Serial("/dev/ttyUSB0")
    time.sleep(3)
    bot = Bot(ser)
    bot.exec_procedure([
        bot.zrot(degrees=40, in_millis=1000),
        bot.zrot(degrees=-40, in_millis=1000),
        bot.backarm(degrees=0, in_millis=2000),
        bot.forarm(degrees=0, in_millis=2000),
        bot.gripper(degrees=0, in_millis=2000)
    ])
    time.sleep(3.1)
    bot.exec_procedure([
        bot.zrot(degrees=0, in_millis=2000),
        bot.backarm(degrees=60, in_millis=1000),
        bot.backarm(degrees=-60, in_millis=1000),
        bot.forarm(degrees=0, in_millis=2000),
        bot.gripper(degrees=0, in_millis=2000)
    ])
    time.sleep(3.1)
    bot.exec_procedure([
        bot.zrot(degrees=0, in_millis=2000),
        bot.backarm(degrees=0, in_millis=2000),
        bot.forarm(degrees=60, in_millis=1000),
        bot.forarm(degrees=-60, in_millis=1000),
        bot.gripper(degrees=0, in_millis=2000)
    ])
    time.sleep(3.1)
    bot.exec_procedure([
        bot.zrot(degrees=0, in_millis=2000),
        bot.backarm(degrees=0, in_millis=2000),
        bot.forarm(degrees=0, in_millis=2000),
        bot.gripper(degrees=40, in_millis=1000),
        bot.gripper(degrees=-40, in_millis=1000)
    ])
    time.sleep(3.1)
    ser.close()

if __name__ == '__main__':
    test_single_motor_movement()