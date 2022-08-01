from maix import nn, camera, display, image

model = "model-8762.awnn.mud"
labels = ['Left', 'Zebra']

def main():
    camera.config(size=(224, 224))

    print("-- load model:", model)
    m = nn.load(model)
    print("-- load ok")

    while True:
        img = camera.capture()
        out = m.forward(img)
        out = nn.F.softmax(out)
        msg = "{:.2f}: {}".format(out.max(), labels[out.argmax()])
        display.show(img.draw_string(2, 2, msg, scale = 1.2, color = (255, 0, 0), thickness = 2))

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

