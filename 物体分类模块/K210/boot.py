

import sensor, image, lcd, time
import KPU as kpu
import gc, sys
from machine import UART
from fpioa_manager import fm

#屏幕显示
def lcd_show_except(e):
    import uio
    err_str = uio.StringIO()
    sys.print_exception(e, err_str)
    err_str = err_str.getvalue()
    img = image.Image(size=(224,224))
    img.draw_string(0, 10, err_str, scale=1, color=(0xff,0x00,0x00))
    lcd.display(img)

def main(labels = None, model_addr="/sd/m.kmodel", sensor_window=(224, 224), lcd_rotation=0, sensor_hmirror=False, sensor_vflip=False):

    #摄像头初始化
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_windowing(sensor_window)
    sensor.set_hmirror(sensor_hmirror)
    sensor.set_vflip(sensor_vflip)
    sensor.run(1)

    #LCD屏幕初始化
    lcd.init(type=1)
    lcd.rotation(lcd_rotation)
    lcd.clear(lcd.WHITE)

    #串口初始化
    fm.register(10,fm.fpioa.UART1_TX,force=True)
    fm.register(11,fm.fpioa.UART1_RX,force=True)
    uart_A=UART(UART.UART1,115200,8,None,1,timeout=1000,read_buf_len=4096)

    if not labels:
        with open('labels.txt','r') as f:
            exec(f.read())
    if not labels:
        print("no labels.txt")
        img = image.Image(size=(320, 240))
        img.draw_string(90, 110, "no labels.txt", color=(255, 0, 0), scale=2)
        lcd.display(img)
        return 1
    try:
        img = image.Image("startup.jpg")
        lcd.display(img)
    except Exception:
        img = image.Image(size=(320, 240))
        img.draw_string(90, 110, "loading model...", color=(255, 255, 255), scale=2)
        lcd.display(img)

    #加载模型
    task = kpu.load(model_addr)

    try:
        while(True):
            img = sensor.snapshot()
            t = time.ticks_ms()
            fmap = kpu.forward(task, img)
            t = time.ticks_ms() - t
            plist=fmap[:]
            pmax=max(plist)#得到相似程度最高的值下标
            max_index=plist.index(pmax)
            img.draw_string(0,0, "%.2f : %s" %(pmax, labels[max_index].strip()), scale=2)
            img.draw_string(0, 200, "t:%dms" %(t), scale=2)
            lcd.display(img)
            if pmax >=0.8:
                uart_A.write(labels[max_index]+'\r\n')
    except Exception as e:
        raise e
    finally:
        kpu.deinit(task)#删除模型释放内存

#命令行模式调试专用函数
if __name__ == "__main__":
    try:
        labels = ["left", "zebra"]
        # main(labels=labels, model_addr=0x300000)
        main(labels=labels, model_addr="/sd/m.kmodel")
    except Exception as e:
        sys.print_exception(e)
        lcd_show_except(e)
    finally:
        gc.collect()
