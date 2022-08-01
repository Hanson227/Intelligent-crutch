from maix import nn, camera, display, image,camera, mjpg, utils
import serial 


model = "model-6285.awnn.mud"
labels = ['Zebra', 'Left']

queue = mjpg.Queue(maxsize=8)
mjpg.MjpgServerThread(
    "0.0.0.0", 18811, mjpg.BytesImageHandlerFactory(q=queue)).start()

ser = serial.Serial("/dev/ttyS1",9600)
ser.setDTR(False)
ser.setRTS(False)

def main():
    camera.config(size=(224, 224))
    
    print("-- load model:", model)
    m = nn.load(model)
    print("-- load ok")

    while True:
        img = camera.capture()
        jpg = utils.rgb2jpg(img.convert("RGB").tobytes(), img.width, img.height)
        queue.put(mjpg.BytesImage(jpg))
        img = camera.capture()
        out = m.forward(img)
        out = nn.F.softmax(out)
        msg = "{:.2f}: {}".format(out.max(), labels[out.argmax()])
        display.show(img.draw_string(2, 2, msg, scale = 1.2, color = (255, 0, 0), thickness = 2))
        if out.argmax()==0:
            ser.write(b'zebra')
        else:
            ser.write(b'Left')


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        import traceback, time
        msg = traceback.format_exc()
        print(msg)
        img = image.new(size = (240, 240), color = (255, 0, 0), mode = "RGB")
        img.draw_string(0, 0, msg, scale = 0.8, color = (255, 255, 255), thickness = 1)
        display.show(img)
        time.sleep(20)

