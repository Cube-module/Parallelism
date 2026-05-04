import queue
import threading
import cv2
import time

import os
import logging

# в puthon деструктор вызывается не после выхода из поля видимости, а только тогда когда нет больше ссылок на объект

# создаем папку log
os.makedirs("log", exist_ok=True)

# настраиваем логирование
logging.basicConfig(
    filename="log/app.log",
    level=logging.INFO, # debug не принимает остальное принимает
    format="%(asctime)s %(levelname)s: %(message)s" # время уровень и сообщение
)

# абстракт класс
class Sensor:
    def get(self):
        raise NotImplementedError("class doesn't have metod get()")
    
# класс датчиков
class SensorX(Sensor):
    def __init__(self, delay):
        self.delay = delay
        self.data = 0

    def get(self):
        time.sleep(self.delay)
        self.data += 1
        return self.data
    
# класс камеры
class SensorCam(Sensor):
    # конструктор
    def __init__(self, cam_id=0):
        # создаем объект для считывания данных с камеры (0 - вебка)
        self.cap = cv2.VideoCapture(cam_id) # поле cap
    
        # проверка на открытие камеры 
        if (self.cap.isOpened() == False):
            raise RuntimeError("camera doesn't open")
        
    def get(self):
        # получаем индикатр и кадр
        success, frame = self.cap.read()

        # получилось ли считать кадр
        if (success == False):
            return None

        return frame

    # деструктор
    def __del__(self):
        # проверка: создалось ли поле cap
        if hasattr(self, "cap"):
            # освобождаем видеопоток и отключаем камеру
            self.cap.release()


def sensor_worker(sensor, q, stop_event):
    """
    Поток для обработки одного датчика.

    Параметры:
    sensor - объект датчика (SensorCam или SensorX)
    q - очередь (queue.Queue) для передачи данных
    stop_event - флаг остановки потока (threading.Event)

    Функция потока датчика:
    читает данные и кладёт последнее значение в очередь,
    работает пока stop_event == False.
    """
    while(stop_event.is_set() == False):

        # получаем данные с сенсора
        data = sensor.get()

        # если данных нет пропускаем итерацию
        if (data is None):
            continue

        # если очередь заполнена удаляем (если оказалось пустой то игнорируем)
        if q.full():
            try:
                # удаляем старый элемент (без ожидания пока он появится)
                q.get_nowait()
            except queue.Empty:
                # ничего не делать (пустая операция)
                pass
                
        # кладем данные в очередь
        q.put(data)


# класс отображения 
class WindowImage:
    def __init__(self, fps):
        self.window_name = "Sensors"
        self.delay_ms = int(1000 / fps)

        try:
            # создаем окно в системе с именем window_name
            cv2.namedWindow(self.window_name)
            # логируем 
            logging.info("Window created")
        except Exception as e:
            logging.error("Window init error: " + str(e))
            raise

    def show(self, img):
        try:
            # выводим картинку 
            cv2.imshow(self.window_name, img)

            # ставим задержку на обновление кадров и проверку нажатия клавишь, получаем код клавиши нажатой
            key = cv2.waitKey(self.delay_ms)

            # обработка нажатия q
            if key == ord("q"):
                return False

            return True

        except Exception as e:
            logging.error("Window show error: " + str(e))
            return False

    def __del__(self):
        try:
            # уничтожаем окно в системе
            cv2.destroyWindow(self.window_name)
            logging.info("Window destroyed")
        except Exception as e:
            logging.error("Window destroy error: " + str(e))


def main():
    # объект-флаг потоков
    stop_event = threading.Event()

    # 4 очереди для каждого датчика 
    q_cam = queue.Queue(maxsize=1) # камера
    q100 = queue.Queue(maxsize=1)
    q10 = queue.Queue(maxsize=1)
    q1 = queue.Queue(maxsize=1)

    # объекты классов датчиков
    cam = SensorCam(0)

    sensor100 = SensorX(0.01)
    sensor10 = SensorX(0.1)
    sensor1 = SensorX(1)

    # объект класса отображения
    window = WindowImage(30)

    # создаем объекты потоков
    threads = [
        threading.Thread(target=sensor_worker, args=(cam, q_cam, stop_event)),
        threading.Thread(target=sensor_worker, args=(sensor100, q100, stop_event)),
        threading.Thread(target=sensor_worker, args=(sensor10, q10, stop_event)),
        threading.Thread(target=sensor_worker, args=(sensor1, q1, stop_event))
    ]

    # запускаем потоки 
    for t in threads:
        t.start()

    # переменные для хранения последних значений
    last100 = 0
    last10 = 0
    last1 = 0


    while True:
        # если кадра нет то рисовать нечего
        if q_cam.empty():
            continue

        # достаем кадр
        frame = q_cam.get()

        # если в очереди что-то есть достаем
        if q100.empty() == False:
            last100 = q100.get()

        if q10.empty() == False:
            last10 = q10.get()

        if q1.empty() == False:
            last1 = q1.get()

        # рисуем на кадре данные датчиков (BGR)
        cv2.putText(frame, "Sensor 100 Hz: " + str(last100), (20, 40),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

        cv2.putText(frame, "Sensor 10 Hz: " + str(last10), (20, 80),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

        cv2.putText(frame, "Sensor 1 Hz: " + str(last1), (20, 120),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        
        # отображение кадра
        if window.show(frame) == False:
            break
    # меняем флаг потоков 
    stop_event.set()

    # убираем потоки
    for t in threads:
        t.join()

    # выключаем камеру
    cam.cap.release()
    # уничтожаем окна
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()