import sensor, image, time

#设置核函数滤波，核内每个数值值域为[-128,127],核需为列表或元组
kernel_size = 1 # kernel width = (size*2)+1, kernel height = (size*2)+1
kernel = [-1, -1, -1,\
          -1, +8, -1,\
          -1, -1, -1]
# 这个一个高通滤波器。见这里有更多的kernel
# http://www.fmwconcepts.com/imagemagick/digital_image_filtering.pdf
thresholds = [(100, 255)] # grayscale thresholds设置阈值

sensor.reset() # 初始化 sensor.
#初始化摄像头

enable_lens_corr = False

sensor.set_pixformat(sensor.GRAYSCALE) # or sensor.RGB565
#设置图像色彩格式，有RGB565色彩图和GRAYSCALE灰度图两种

sensor.set_framesize(sensor.QQVGA) # or sensor.QVGA (or others)
#设置图像像素大小
sensor.skip_frames(10) # 让新的设置生效
clock = time.clock() # 跟踪FPS帧率

# 在OV7725 sensor上, 边缘检测可以通过设置sharpness/edge寄存器来增强。
# 注意:这将在以后作为一个函数实现
if (sensor.get_id() == sensor.OV7725):
    sensor.__write_reg(0xAC, 0xDF)
    sensor.__write_reg(0x8F, 0xFF)

min_degree = 0
max_degree = 179

while(True):
    clock.tick() # 追踪两个snapshots()之间经过的毫秒数.
    img = sensor.snapshot() # 拍一张照片，返回图像

    img.morph(kernel_size, kernel)
    img.binary(thresholds)
    #利用binary函数对图像进行分割

    # Erode pixels with less than 2 neighbors using a 3x3 image kernel
    # 腐蚀像素小于2邻居使用3x3图像内核
    img.erode(1, threshold = 2)
    #侵蚀函数erode(size, threshold=Auto)，去除边缘相邻处多余的点。threshold
    #用来设置去除相邻点的个数，threshold数值越大，被侵蚀掉的边缘点越多，边缘旁边
    #白色杂点少；数值越小，被侵蚀掉的边缘点越少，边缘旁边的白色杂点越多。
    if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...

    # `merge_distance`控制附近行的合并。 在0（默认），没有合并。
    # 在1处，任何距离另一条线一个像素点的线都被合并...等等，
    # 因为你增加了这个值。 您可能希望合并线段，因为线段检测会产生大量
    # 的线段结果。

    # `max_theta_diff` 控制要合并的任何两线段之间的最大旋转差异量。
    # 默认设置允许15度。

    for l in img.find_line_segments(merge_distance = 10, max_theta_diff = 10):
        img.draw_line(l.line(), color = (255, 0, 0))
