from choreo import Bot
import unittest

class MockWriter:
    def __init__(self):
        self.data = None
    def write(self,data):
        self.data = data
    def get_written_data(self):
        return self.data


class TestBot(unittest.TestCase):

    def test_creation(self):
        writer = MockWriter()
        bot = Bot(writer)
    
    def test_procedure(self):
        writer = MockWriter()
        bot = Bot(writer)
        bot.exec_procedure(
            [
                bot.zrot(degrees=40, in_millis=3000)
            ]
        )

        self.assertEqual(
            b'\x01\x02\x01\x00\x00\x28\x0B\xB8',
            writer.get_written_data()
        )

        bot.exec_procedure(
            [
                bot.zrot(degrees=30, in_millis=2000)
            ]
        )

        self.assertEqual(
            b'\x01\x02\x01\x00\x00\x1E\x07\xD0',
            writer.get_written_data()
        )

    def test_multi_motor_procedure(self):

        writer = MockWriter()
        bot = Bot(writer)

        bot.exec_procedure(
            [
                bot.zrot(degrees=30, in_millis=2000),
                bot.backarm(degrees=20, in_millis=4000)
            ]
        )

        self.assertEqual(
            b'\x02\x01\x01\x00\x00\x14\x0F\xA0\x02\x01\x00\x00\x1E\x07\xD0',
            writer.get_written_data()
        )

    def test_multiple_procedures_per_motor(self):
        writer = MockWriter()
        bot = Bot(writer)
        bot.exec_procedure(
            [
                bot.zrot(degrees=40, in_millis=3000),
                bot.zrot(degrees=40, in_millis=3000),
            ]
        )

        self.assertEqual(
            b'\x01\x02\x02\x00\x00\x28\x0B\xB8\x01\x00\x28\x0B\xB8',
            writer.get_written_data()
        )

    def test_procedure(self):
        writer = MockWriter()
        bot = Bot(writer)
        bot.exec_procedure(
            [
                bot.zrot(degrees=40, in_millis=3000),
                bot.backarm(degrees=40, in_millis=3000),
                bot.forarm(degrees=40, in_millis=3000),
                bot.gripper(degrees=40, in_millis=3000)
            ]
        )

        self.assertEqual(
            writer.get_written_data(),
            b'\x04\x00\x01\x00\x00\x28\x0B\xB8\x01\x01\x00\x00\x28\x0B\xB8\x02\x01\x00\x00\x28\x0B\xB8\x03\x01\x00\x00\x28\x0B\xB8',
        )

   
if __name__ == '__main__':
    unittest.main()
