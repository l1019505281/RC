from tkinter import *
from tkinter import ttk
import tkinter as tk
from PIL import Image, ImageTk
import serial
from serial import *
import numpy as np
import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
from struct import pack,unpack
from time import sleep
import threading

Bezier_P = 3 #贝塞尔曲线次数
Bezier_C = 0 #用于计数，是贝塞尔曲线的第几个点
ClickX = [0,0,0,0] #返回鼠标点击x坐标
ClickY = [0,0,0,0] #返回鼠标点击y坐标
COM_chose = "COM6"  #串口默认选择6
Rec_data = [0,0,0,0,0,0]    #接收电脑反馈数据的列表

Oscilloscope_x = [0]    #示波器的x轴
Oscilloscope_y = [0]    #示波器的y轴
Oscilloscope_t = 0      #示波器计数

#/////////////////////GUI功能//////////////////////////////#


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
                    串口相关函数
                ↓↓↓↓↓↓↓↓↓↓
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
def refresh_data(): ###用timer0.01秒进入一次该函数，接收数据
    global timer
    try:
        
        serialPort=COM_chose #串口
        baudRate=115200 #波特率
        ser=serial.Serial(serialPort,baudRate)
        
        Rec_data[0]=ser.readline()#x
        Rec_data[1]=ser.readline()#y
        Rec_data[2]=ser.readline()#w
        Rec_data[3]=ser.readline()#point_counter
        Rec_data[4]=ser.readline()#vel
        Rec_data[5]=ser.readline()
        Rec_data[0] = Rec_data[0].decode()#将数据从btyes(b"")类型解码成字符串类型
        Rec_data[1] = Rec_data[1].decode()
        Rec_data[2] = Rec_data[2].decode()
        Rec_data[3] = Rec_data[3].decode()
        Rec_data[4] = Rec_data[4].decode()
        Rec_data[5] = Rec_data[5].decode()
        
        Disp1_x.delete(0.0,END)           #显示到tab1的display中
        Disp1_x.insert(END,Rec_data[0])
        Disp1_y.delete(0.0,END)
        Disp1_y.insert(END,Rec_data[1])
        Disp1_w.delete(0.0,END)
        Disp1_w.insert(END,Rec_data[2])
        
        
        path_point_counter.delete(0.0,END)
        path_point_counter.insert(END,Rec_data[3])
        MR1_vel.delete(0.0,END)
        MR1_vel.insert(END,Rec_data[4])
        Disp1_Vw.delete(0.0,END)
        Disp1_Vw.insert(END,Rec_data[5])

        Oscilloscope_update(Rec_data[4])
        
        '''
        print("---->\n")
        print(Rec_data[0])
        print(Rec_data[1])
        print(Rec_data[2])
        print("----\n")
        '''
        
    except Exception as e:
        serialPort=COM_chose
        print("请选择检查是否选择对串口,错误反馈：")
        print(e)
    
    timer = threading.Timer(0.1, refresh_data)  #每隔0.1秒进入一次refresh_data函数
    timer.start()

def serial_write_data(data):                #发送数据（float类型）  一般用于传输所需数据
    try:
        serialPort=COM_chose #串口
        baudRate=115200 #波特率
        ser=serial.Serial(serialPort,baudRate,timeout=0.5)
        float_bytes = pack('f', data)       #将float类型pack
        ser.write(float_bytes)
    except Exception as e:
        print("请选择检查是否选择对串口,错误反馈：")
        print(e)
    
def serial_write_tag(data):                 #发送数据头之类的（如"x0D"，为字符串，直接用.encode()）
    try:
        serialPort=COM_chose #串口
        baudRate=115200 #波特率
        ser=serial.Serial(serialPort,baudRate,timeout=0.5)
        ser.write(data.encode('utf-8'))     #要用encode转成bytes类型
    except Exception as e:
        print("请选择检查是否选择对串口,错误反馈：")
        print(e)

def com_chose(*args): #选择串口
    global COM_chose
    COM_chose = COMChosen.get()






'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
                    发送、更新参数相关函数
                ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

def update_PID(): #更新PID参数
    PIDdata = [0,0,0]
    for each,i in zip([P,I,D],range(3)):
        PIDdata[i] = each.get()
        #print(PIDdata[i])
    serial_write_tag("\x0D\x0B")            #发送数据头
    serial_write_tag("\x61")                #判断数据类型

    for i in range(3):
        serial_write_data(float(PIDdata[i].encode()))
        serial_write_tag('\n')
    serial_write_tag("\x0B\x0D")            #数据尾
    #print(PIDdata)
    
def update(): #更新所有数据       未完成
    for i in range(6):
        serial_write_data(i)
    print("update data done!")





    
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
                    路径规划相关函数（tab3中
                ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

def change_point():#鼠标点击不精确时手动输入修改点坐标
    global ClickX
    global ClickY
    ClickX[0] = int(Bezier_point1x.get(0.0,END)) #得到输入框中的数字
    ClickX[1] = int(Bezier_point2x.get(0.0,END))
    ClickX[2] = int(Bezier_point3x.get(0.0,END))
    ClickX[3] = int(Bezier_point4x.get(0.0,END))

    ClickY[0] = int(Bezier_point1y.get(0.0,END))
    ClickY[1] = int(Bezier_point2y.get(0.0,END))
    ClickY[2] = int(Bezier_point3y.get(0.0,END))
    ClickY[3] = int(Bezier_point4y.get(0.0,END)) #得到输入框中的数字


def clear_point(): #点坐标清零
    global ClickX
    global ClickY
    global Bezier_C

    Bezier_C = 0 #贝塞尔曲线的第几个点的计数清0
    ClickX[0] = 0   #点坐标清零
    ClickX[1] = 0
    ClickX[2] = 0
    ClickX[3] = 0

    ClickY[0] = 0
    ClickY[1] = 0
    ClickY[2] = 0
    ClickY[3] = 0

    Bezier_point1x.delete(0.0,END)      #用0代替输入框中的数字
    Bezier_point2x.delete(0.0,END)
    Bezier_point3x.delete(0.0,END)
    Bezier_point4x.delete(0.0,END)
    Bezier_point1x.insert(END,ClickX[0])
    Bezier_point2x.insert(END,ClickX[1])
    Bezier_point3x.insert(END,ClickX[2])
    Bezier_point4x.insert(END,ClickX[3])

    Bezier_point1y.delete(0.0,END)
    Bezier_point2y.delete(0.0,END)
    Bezier_point3y.delete(0.0,END)
    Bezier_point4y.delete(0.0,END)
    Bezier_point1y.insert(END,ClickY[0])
    Bezier_point2y.insert(END,ClickY[1])
    Bezier_point3y.insert(END,ClickY[2])
    Bezier_point4y.insert(END,ClickY[3]) #用0代替输入框中的数字

def clear_canvas():
    canvas.delete("path","point")

def drawPic(): #显示计算出来后的路线
    python_red = "RED"

    PNb = int(Point_number.get())  #得到想要计算出离散点的个数
    
    
    t = np.linspace(0, 1, PNb)  #贝塞尔曲线公式中的t
    t2 = np.linspace(0, 1, int(PNb/2)) #同上

    vel = Set_vel.get()#得到想要的速度
    
    if Bezier_P == 2:  #如果选择贝塞尔二次曲线
        X2 = ClickX[0]*pow((1-t2),2)+2*ClickX[1]*t2*(1-t2)+ClickX[2]*pow(t2,2)  #贝塞尔曲线计算公式
        Y2 = ClickY[0]*pow((1-t2),2)+2*ClickY[1]*t2*(1-t2)+ClickY[2]*pow(t2,2)
        X2 = X2.tolist() #转化成列表
        X2.append(X2[-1])#在列表后面加多一个重复的点，这样后面绘图才不会超列表（如果不加那么i为最后一个数字时，i+1将超出列表
        Y2 = Y2.tolist()
        Y2.append(Y2[-1])#同理
        for i in range(int(PNb/2)):
            canvas.create_line( int(X2[i]/20)+25, int(Y2[i]/20)+25, int(X2[i+1]/20)+25, int(Y2[i+1]/20)+25, fill = python_red ,tag = "point")#根据所选点个数与计算结果，绘制出曲线（/20因为屏幕像素不够，+25是根据MR1的坐标原点所定
    elif Bezier_P == 3:  #如果选择贝塞尔三次曲线
        X = ClickX[0]*pow((1-t),3)+3*ClickX[1]*t*pow((1-t),2)+3*ClickX[2]*(1-t)*pow(t,2)+ClickX[3]*pow(t,3)#贝塞尔曲线计算公式
        Y = ClickY[0]*pow((1-t),3)+3*ClickY[1]*t*pow((1-t),2)+3*ClickY[2]*(1-t)*pow(t,2)+ClickY[3]*pow(t,3)#ClickX、Y为鼠标点击后的坐标
        X = X.tolist()
        X.append(X[-1])
        Y = Y.tolist()
        Y.append(Y[-1])
        for i in range(PNb):
            canvas.create_line( int(X[i]/20)+25, int(Y[i]/20)+25,
                                int(X[i+1]/20)+25, int(Y[i+1]/20)+25,
                                fill = python_red ,tag = "point")#根据所选点个数与计算结果，绘制出曲线
    
    if Bezier_P == 2:                   #打印出计算出离散点的坐标（二次
        point_output.delete(0.0,END)
        for j in range(int(PNb/2)):
            show_text = "{"+str(X2[j])+","+str(Y2[j])+",0,"+vel+"}," #根据在MDK5中的结构体，改变不同的输出形式
            point_output.insert(END,show_text)
            point_output.insert(END,"\n")
            #print(show_text)   
    elif Bezier_P == 3:                  #打印出计算出离散点的坐标（三次
        point_output.delete(0.0,END)    
        for j in range(PNb):
            show_text = "{"+str(X[j])+","+str(Y[j])+",0,"+vel+"},"
            point_output.insert(END,show_text)
            point_output.insert(END,"\n")
            #print(show_text)

    
def showPic(): #显示需要走的路径，如地图所示
    Mx = [0,0,1500,1500,0,0,1500,1500,725,725,4905,4905]#路径各点x坐标
    My = [0,750,750,2250,2250,3750,3750,5250,5250,8500,8500,7485]#y
    
    python_green = "#476042"
    for i in range(1,12):
        canvas.create_line( int(Mx[i-1]/20)+25, int(My[i-1]/20)+25,
                            int(Mx[i]/20)+25, int(My[i]/20)+25,
                            fill = python_green ,tag = "path")#画出路径，电脑屏幕像素受限，要按比例缩小

def click(event):           #鼠标点击画布事件 #用于鼠标点击地图做贝塞尔曲线，规划地图
    print("x:%d,y:%d"%(event.x,event.y))
    global Bezier_C
    global ClickX
    global ClickY
    if Bezier_P == 2:
        if Bezier_C == 2:
            ClickX[Bezier_C] = (event.x-25)*20
            ClickY[Bezier_C] = (event.y-25)*20
            Bezier_C = 0
        elif Bezier_C == 3:                 #当开始选择三次，点到第三个点时，Berzier_C为3，再改曲线次方时会出现bug
            Bezier_C = 0
            ClickX[Bezier_C] = (event.x-25)*20
            ClickY[Bezier_C] = (event.y-25)*20
            Bezier_C += 1
        else:
            ClickX[Bezier_C] = (event.x-25)*20
            ClickY[Bezier_C] = (event.y-25)*20
            Bezier_C += 1
    elif Bezier_P ==3:
        if Bezier_C == 3:
            ClickX[Bezier_C] = (event.x-25)*20
            ClickY[Bezier_C] = (event.y-25)*20
            Bezier_C = 0
        else:
            ClickX[Bezier_C] = (event.x-25)*20
            ClickY[Bezier_C] = (event.y-25)*20
            Bezier_C += 1
    else:
        print("ac click xy err")
    Bezier_point1x.delete(0.0,END)
    Bezier_point2x.delete(0.0,END)
    Bezier_point3x.delete(0.0,END)
    Bezier_point4x.delete(0.0,END)
    Bezier_point1x.insert(END,ClickX[0])
    Bezier_point2x.insert(END,ClickX[1])
    Bezier_point3x.insert(END,ClickX[2])
    Bezier_point4x.insert(END,ClickX[3])
    '''''''''''''''
    将鼠标点击的点显示在输入框中
    '''''''''''''''
    Bezier_point1y.delete(0.0,END)
    Bezier_point2y.delete(0.0,END)
    Bezier_point3y.delete(0.0,END)
    Bezier_point4y.delete(0.0,END)
    Bezier_point1y.insert(END,ClickY[0])
    Bezier_point2y.insert(END,ClickY[1])
    Bezier_point3y.insert(END,ClickY[2])
    Bezier_point4y.insert(END,ClickY[3])
    
def chose_bezier(): #选择贝塞尔曲线的次数
    global Bezier_P
    Bezier_P = Bezier_chose.get()
    print(Bezier_P)

def client_exit(): #退出程序，没什么卵用
    exit()


    
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
                   示波器相关函数（tab5中
                ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
def Oscilloscope_update(vel):
    
    global Oscilloscope_t,Oscilloscope_x,Oscilloscope_y #将x,y,t设为全局变量
    vel = int(float(vel))                       #单片机传输回来的数据为"1.3"这类型的字符浮点型，不可以使用int()，得先转化成float
    Oscilloscope_t += 10                        #每次反馈数据x轴增加的距离
    Oscilloscope_x.append(Oscilloscope_t)       #x轴递增
    Oscilloscope_y.append(int(vel/4))           #将y轴坐标存入列表
    
    Oscilloscope_canvas.create_line(Oscilloscope_x[-2],-Oscilloscope_y[-2]+300,
                                    Oscilloscope_x[-1],-Oscilloscope_y[-1]+300,
                                    fill = "RED" ,tag = "Osc")      #画出波形
    if Oscilloscope_t >= 1200:  #当波形到屏幕最右方时，清空屏幕
        Oscilloscope_t = 0
        Oscilloscope_canvas.delete("Osc")   #清空波形
        Oscilloscope_x = [0]                #将x坐标设回起点
        Oscilloscope_y = [Oscilloscope_y[-1]]#y坐标不变
    #print(Oscilloscope_y[-1])
    
#/////////////////////GUI功能//////////////////////////////#





if __name__ == '__main__':    
    #////////////////////GUI的布局////////////////////////////#
    root = Tk()
    root.geometry("1280x720")   #窗口大小
    root.title("上位机")        #窗口标题

    # 创建Edit菜单，下面有一个Undo菜单
    edit = Menu(root)
    edit.add_command(label='更新全部数据',command=update)#菜单选项信息
    edit.add_command(label='未开发')
    edit.add_command(label='未完成')
    root['menu'] = edit

    # 显示按钮，并使用grid布局
    quitButton = Button(root, text='Quit', command=client_exit) #退出按钮
    quitButton.pack(side = 'bottom')


    # Tab菜单-----------------------------------------
    tabControl = ttk.Notebook()          

    tab1 = ttk.Frame(tabControl)  # 创建一个容器，其父容器为win
    tabControl.add(tab1, text='调参')      # Add the tab

    tab2 = ttk.Frame(tabControl)
    tabControl.add(tab2, text='串口')      

    tab3 = ttk.Frame(tabControl)           
    tabControl.add(tab3, text='路径规划')     

    tab4 = ttk.Frame(tabControl)            
    tabControl.add(tab4, text='地图')      

    tab5 = ttk.Frame(tabControl)            
    tabControl.add(tab5, text='示波器')
    
    tabControl.pack(expand=1, fill="both")  
    # Tab菜单-----------------------------------------




    

    # Tab1内容-----------------------------------------
    
    #////////////////参数容器
    Para1lab = ttk.LabelFrame(tab1, text='Para1')
    Para1lab.grid(column=0, row=0, padx=8, pady=4)

    tk.Label(Para1lab, text="a_papram::",bg='white',font=('Arial', 10),width=12, height=1).grid(column=0, row=0,sticky="w")
    tk.Label(Para1lab, text="b_papram::",bg='white',font=('Arial', 10),width=12, height=1).grid(column=0, row=1,sticky="w")
    tk.Label(Para1lab, text="c_papram::",bg='white',font=('Arial', 10),width=12, height=1).grid(column=0, row=2,sticky="w")
    tk.Label(Para1lab, text="d_papram::",bg='white',font=('Arial', 10),width=12, height=1).grid(column=0, row=3,sticky="w")
    tk.Label(Para1lab, text="e_papram::",bg='white',font=('Arial', 10),width=12, height=1).grid(column=0, row=4,sticky="w")
    tk.Label(Para1lab, text="f_papram::",bg='white',font=('Arial', 10),width=12, height=1).grid(column=0, row=5,sticky="w")
    tk.Label(Para1lab, text="g_papram::",bg='white',font=('Arial', 10),width=12, height=1).grid(column=0, row=6,sticky="w")
    tk.Label(Para1lab, text="h_papram::",bg='white',font=('Arial', 10),width=12, height=1).grid(column=0, row=7,sticky="w")

    Para_a = StringVar()
    Para_a = ttk.Entry(Para1lab, width=24, textvariable=Para_a)
    Para_a.grid(column=1, row=0, sticky='W')
    
    Para_b = StringVar()
    Para_b = ttk.Entry(Para1lab, width=24, textvariable=Para_b)
    Para_b.grid(column=1, row=1, sticky='W')

    Para_c = StringVar()
    Para_c = ttk.Entry(Para1lab, width=24, textvariable=Para_c)
    Para_c.grid(column=1, row=2, sticky='W')

    Para_d = StringVar()
    Para_d = ttk.Entry(Para1lab, width=24, textvariable=Para_d)
    Para_d.grid(column=1, row=3, sticky='W')

    Para_e = StringVar()
    Para_e = ttk.Entry(Para1lab, width=24, textvariable=Para_d)
    Para_e.grid(column=1, row=4, sticky='W')

    Para_f = StringVar()
    Para_f = ttk.Entry(Para1lab, width=24, textvariable=Para_f)
    Para_f.grid(column=1, row=5, sticky='W')

    Para_g = StringVar()
    Para_g = ttk.Entry(Para1lab, width=24, textvariable=Para_g)
    Para_g.grid(column=1, row=6, sticky='W')

    Para_h = StringVar()
    Para_h = ttk.Entry(Para1lab, width=24, textvariable=Para_h)
    Para_h.grid(column=1, row=7, sticky='W')

    ParaButton = ttk.Button(Para1lab, text='更新', command=update_PID)
    ParaButton.grid(column=1, row=8)
    #////////////////

    #////////////////显示容器MR1
    Disp1 = ttk.LabelFrame(tab1, text='Display.MR1')
    Disp1.grid(column=0, row=1,columnspan=2)

    tk.Label(Disp1, text="MR1_pos_x:",font=('Arial', 10),width=12, height=1).grid(column=0, row=0,sticky="w")
    tk.Label(Disp1, text="MR1_pos_y:",font=('Arial', 10),width=12, height=1).grid(column=0, row=1,sticky="w")
    tk.Label(Disp1, text="MR1_pos_w:",font=('Arial', 10),width=12, height=1).grid(column=0, row=2,sticky="w")
    tk.Label(Disp1, text="MR1_vel_x:",font=('Arial', 10),width=12, height=1).grid(column=0, row=3,sticky="w")
    tk.Label(Disp1, text="MR1_vel_y:",font=('Arial', 10),width=12, height=1).grid(column=0, row=4,sticky="w")
    tk.Label(Disp1, text="MR1_vel_w:",font=('Arial', 10),width=12, height=1).grid(column=0, row=5,sticky="w")

    tk.Label(Disp1, text="当前离散点:",font=('Arial', 10),width=12, height=1).grid(column=2, row=0,sticky="w")
    tk.Label(Disp1, text="车实际速度:",font=('Arial', 10),width=12, height=1).grid(column=2, row=1,sticky="w")
    
    Disp1_x = Text(Disp1, width=30, height=1)
    Disp1_x.grid(column=1, row=0,sticky="w")
    Disp1_x.insert(END,0)

    Disp1_y = Text(Disp1, width=30, height=1)
    Disp1_y.grid(column=1, row=1,sticky="w")
    Disp1_y.insert(END,0)

    Disp1_w = Text(Disp1, width=30, height=1)
    Disp1_w.grid(column=1, row=2,sticky="w")
    Disp1_w.insert(END,0)

    Disp1_Vx = Text(Disp1, width=30, height=1)
    Disp1_Vx.grid(column=1, row=3,sticky="w")
    Disp1_Vx.insert(END,0)

    Disp1_Vy = Text(Disp1, width=30, height=1)
    Disp1_Vy.grid(column=1, row=4,sticky="w")
    Disp1_Vy.insert(END,0)

    Disp1_Vw = Text(Disp1, width=30, height=1)
    Disp1_Vw.grid(column=1, row=5,sticky="w")
    Disp1_Vw.insert(END,0)

    path_point_counter = Text(Disp1, width=30, height=1)
    path_point_counter.grid(column=3, row=0,sticky="w")
    path_point_counter.insert(END,0)

    MR1_vel = Text(Disp1, width=30, height=1)
    MR1_vel.grid(column=3, row=1,sticky="w")
    MR1_vel.insert(END,0)

    
    #////////////////

    #////////////////显示容器MR2
    Disp2 = ttk.LabelFrame(tab1, text='Display.MR2')
    Disp2.grid(column=0, row=2,columnspan=2)

    tk.Label(Disp2, text="MR2_pos_x:",font=('Arial', 10),width=12, height=1).grid(column=0, row=0,sticky="w")
    tk.Label(Disp2, text="MR2_pos_y:",font=('Arial', 10),width=12, height=1).grid(column=0, row=1,sticky="w")
    tk.Label(Disp2, text="MR2_pos_w:",font=('Arial', 10),width=12, height=1).grid(column=0, row=2,sticky="w")
    tk.Label(Disp2, text="MR2_vel_x:",font=('Arial', 10),width=12, height=1).grid(column=0, row=3,sticky="w")
    tk.Label(Disp2, text="MR2_vel_y:",font=('Arial', 10),width=12, height=1).grid(column=0, row=4,sticky="w")
    tk.Label(Disp2, text="MR2_vel_w:",font=('Arial', 10),width=12, height=1).grid(column=0, row=5,sticky="w")

    tk.Label(Disp2, text="MR2_motor_vel:",font=('Arial', 10),width=12, height=1).grid(column=2, row=0,sticky="w")
    tk.Label(Disp2, text="MR2_motor_pos:",font=('Arial', 10),width=12, height=1).grid(column=2, row=1,sticky="w")
    tk.Label(Disp2, text="MR2_motor_cur:",font=('Arial', 10),width=12, height=1).grid(column=2, row=2,sticky="w")
        
    Disp2_x = Text(Disp2, width=30, height=1)
    Disp2_x.grid(column=1, row=0,sticky="w")
    Disp2_x.insert(END,0)

    Disp2_y = Text(Disp2, width=30, height=1)
    Disp2_y.grid(column=1, row=1,sticky="w")
    Disp2_y.insert(END,0)

    Disp2_w = Text(Disp2, width=30, height=1)
    Disp2_w.grid(column=1, row=2,sticky="w")
    Disp2_w.insert(END,0)

    Disp2_Vx = Text(Disp2, width=30, height=1)
    Disp2_Vx.grid(column=1, row=3,sticky="w")
    Disp2_Vx.insert(END,0)

    Disp2_Vy = Text(Disp2, width=30, height=1)
    Disp2_Vy.grid(column=1, row=4,sticky="w")
    Disp2_Vy.insert(END,0)

    Disp2_Vw = Text(Disp2, width=30, height=1)
    Disp2_Vw.grid(column=1, row=5,sticky="w")
    Disp2_Vw.insert(END,0)

    Disp2_mot_vel = Text(Disp2, width=30, height=1)
    Disp2_mot_vel.grid(column=3, row=0,sticky="w")
    Disp2_mot_vel.insert(END,0)

    Disp2_mot_pos = Text(Disp2, width=30, height=1)
    Disp2_mot_pos.grid(column=3, row=1,sticky="w")
    Disp2_mot_pos.insert(END,0)

    Disp2_mot_cur = Text(Disp2, width=30, height=1)
    Disp2_mot_cur.grid(column=3, row=2,sticky="w")
    Disp2_mot_cur.insert(END,0)
    
    #////////////////

    #//////////////PID容器
    PIDlab = ttk.LabelFrame(tab1, text='PID')
    PIDlab.grid(column=1, row=0)
    
    tk.Label(PIDlab, text="P:",bg='white',font=('Arial', 10),width=6, height=1).grid(column=0, row=0, sticky='W')
    P = StringVar()
    Pdata = ttk.Entry(PIDlab, width=24, textvariable=P)
    Pdata.grid(column=1, row=0, sticky='W')

    tk.Label(PIDlab, text="I:",bg='white',font=('Arial', 10),width=6, height=1).grid(column=0, row=1, sticky='W')
    I = StringVar()
    Idata = ttk.Entry(PIDlab, width=24, textvariable=I)
    Idata.grid(column=1, row=1, sticky='W')

    tk.Label(PIDlab, text="D:",bg='white',font=('Arial', 10),width=6, height=1).grid(column=0, row=2, sticky='W')
    D = StringVar()
    Ddata = ttk.Entry(PIDlab, width=24, textvariable=D)
    Ddata.grid(column=1, row=2, sticky='W')

    PIDButton = ttk.Button(PIDlab, text='更新', command=update_PID)
    PIDButton.grid(column=1, row=3)
    #//////////////
    
    # -----------------------------------------Tab1内容




    

    # Tab2内容-----------------------------------------
    com_assi= ttk.LabelFrame(tab2, text='serial')
    com_assi.grid(column=0, row=0, padx=8, pady=4)
    COM = StringVar()       #开启下拉框
    COMChosen = ttk.Combobox(com_assi, width=12, textvariable=COM)
    COMChosen['values'] = ('COM1', 'COM2','COM3','COM4','COM5','COM6', 'COM7', 'COM8', 'COM9')#下拉框内容
    COMChosen.bind("<<ComboboxSelected>>", com_chose)#点击后调用函数
    COMChosen.grid(column=0, row=0)
    # -----------------------------------------Tab2内容



    

    # Tab3内容-----------------------------------------
    matplotlib.use('TkAgg')
    
    path= ttk.LabelFrame(tab3, text='path')
    path.grid(column=0, row=0)

    canvas = Canvas(path, bg='white', height=1000, width=700)
    canvas.grid(row=0,column=0)
    canvas.bind( "<Button-1>", click )

    path_key= ttk.LabelFrame(tab3)
    path_key.grid(column=1, row=0,sticky="NW")
    
    path_botton= ttk.LabelFrame(path_key, text='Botton')
    path_botton.grid(column=1, row=0,sticky=NW)
    tk.Label(path_botton, text="点个数:",font=('Arial', 10),width=6, height=1).grid(row=0,column=0,sticky="w")
    Point_number=Entry(path_botton,width=6)
    Point_number.grid(row=0,column=1)
    Point_number.insert(0,'10')
    tk.Label(path_botton, text="速度:",font=('Arial', 10),width=6, height=1).grid(row=1,column=0,sticky="w")
    Set_vel=Entry(path_botton,width=6)
    Set_vel.grid(row=1,column=1)
    Set_vel.insert(0,'400')
    Button(path_botton,text='显示路径',command=showPic).grid(row=3,column=0)
    Button(path_botton,text='路径规划',command=drawPic).grid(row=3,column=1)

    path_bezier_chose= ttk.LabelFrame(path_key, text='Chose Beizer')
    path_bezier_chose.grid(column=2, row=0,sticky=NW)
    Bezier_chose=IntVar()
    Bezier_chose.set(3)
    Radiobutton(path_bezier_chose,variable = Bezier_chose,text = '二次曲线',value = 2).grid(row=0,column=0)
    Radiobutton(path_bezier_chose,variable = Bezier_chose,text = '三次曲线',value = 3).grid(row=1,column=0)
    Button(path_bezier_chose,text='选择完毕',command=chose_bezier).grid(row=2,column=0)
    
    path_chose= ttk.LabelFrame(path_key, text='Display Chosen Point')
    path_chose.grid(column=3, row=0,sticky=NW)
    tk.Label(path_chose, text="第一个点(x,y):",font=('Arial', 10),width=12, height=1).grid(column=0, row=0,sticky="w")
    tk.Label(path_chose, text="第二个点(x,y):",font=('Arial', 10),width=12, height=1).grid(column=0, row=1,sticky="w")
    tk.Label(path_chose, text="第三个点(x,y):",font=('Arial', 10),width=12, height=1).grid(column=0, row=2,sticky="w")
    tk.Label(path_chose, text="第四个点(x,y):",font=('Arial', 10),width=12, height=1).grid(column=0, row=3,sticky="w")

    Button(path_chose,text='手动修改点坐标',command=change_point).grid(row=4,column=1)
    Button(path_chose,text='点坐标清零',command=clear_point).grid(row=4,column=0)

    Bezier_point1x = Text(path_chose, width=6, height=1)
    Bezier_point1x.grid(column=1, row=0,sticky="w")
    Bezier_point1x.insert(END,ClickX[0])

    Bezier_point2x = Text(path_chose, width=6, height=1)
    Bezier_point2x.grid(column=1, row=1,sticky="w")
    Bezier_point2x.insert(END,ClickX[1])

    Bezier_point3x = Text(path_chose, width=6, height=1)
    Bezier_point3x.grid(column=1, row=2,sticky="w")
    Bezier_point3x.insert(END,ClickX[2])

    Bezier_point4x = Text(path_chose, width=6, height=1)
    Bezier_point4x.grid(column=1, row=3,sticky="w")
    Bezier_point4x.insert(END,ClickX[3])

    Bezier_point1y = Text(path_chose, width=6, height=1)
    Bezier_point1y.grid(column=2, row=0,sticky="w")
    Bezier_point1y.insert(END,ClickY[0])

    Bezier_point2y = Text(path_chose, width=6, height=1)
    Bezier_point2y.grid(column=2, row=1,sticky="w")
    Bezier_point2y.insert(END,ClickY[1])

    Bezier_point3y = Text(path_chose, width=6, height=1)
    Bezier_point3y.grid(column=2, row=2,sticky="w")
    Bezier_point3y.insert(END,ClickY[2])

    Bezier_point4y = Text(path_chose, width=6, height=1)
    Bezier_point4y.grid(column=2, row=3,sticky="w")
    Bezier_point4y.insert(END,ClickY[3])

    path_output= ttk.LabelFrame(path_key, text='Output')
    path_output.grid(column=1,row=2,columnspan=3,sticky=W+E+N+S)
    point_output = Text(path_output, width=50, height=20)
    point_output.grid(column=0, row=0,sticky="w")
    Button(path_output,text='清除画布',command=clear_canvas).grid(row=1,column=0)
    
    # -----------------------------------------Tab3内容




    

    # Tab4内容-----------------------------------------
    load = Image.open('map.jpg')
    render= ImageTk.PhotoImage(load)
    img = Label(tab4,image=render)
    img.image = render
    img.place(x=0,y=0)
    #-----------------------------------------Tab4内容





    # Tab5内容-----------------------------------------
    matplotlib.use('TkAgg')
    
    Oscilloscope= ttk.LabelFrame(tab5, text='Oscilloscope')
    Oscilloscope.grid(column=0, row=0)

    Oscilloscope_canvas = Canvas(Oscilloscope, bg='white', height=1000, width=1200)
    Oscilloscope_canvas.grid(row=0,column=0)


    for i in range(10): #画出刻度
        scale = str(-i)
        Oscilloscope_canvas.create_line(0,i*30+300,10,i*30+300, fill = "BLUE" ,tag = "Scale")
        Oscilloscope_canvas.create_text(20,i*30+300,text = scale)

    for i in range(10): #画出刻度
        i = -i
        suzi = str(-i)
        Oscilloscope_canvas.create_line(0,i*30+300,10,i*30+300, fill = "BLUE" ,tag = "Scale")
        Oscilloscope_canvas.create_text(20,i*30+300,text = suzi)
    #canvas.bind( "<Button-1>", click )

    #-----------------------------------------Tab5内容



    
    timer = threading.Timer(1, refresh_data)#初始化并使用timer
    timer.start()


    root.mainloop()

    #////////////////////GUI的布局////////////////////////////#
