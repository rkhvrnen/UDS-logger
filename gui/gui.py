import tkinter
from tkinter import ttk
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure
import numpy as np
import os
import csv_handler
import database_handler

datalist_vars = []
checkboxes = []

root = tkinter.Tk()
root.wm_title("Data")

filelist = os.listdir("data")

fig = Figure(figsize=(20, 10), dpi=100)
ax = fig.add_subplot(1, 1, 1)
x = np.linspace(0, 10, 100)
y = np.sin(x)
ax.plot(x, y)
ax.grid()
#fig.add_axes(ax)

drawing_area = FigureCanvasTkAgg(fig, master=root)
drawing_area.draw()
drawing_area.get_tk_widget().grid(row=0, column=0, rowspan=19, columnspan=8)
#drawing_area.get_tk_widget().pack(side=tkinter.LEFT, fill=tkinter.BOTH, expand=1)

toolbar = NavigationToolbar2Tk(drawing_area, root, pack_toolbar=False)
toolbar.update()
toolbar.grid(row=20, column=0, rowspan=1, columnspan=1) #pack(side=tkinter.LEFT, fill=tkinter.BOTH, expand=1)

def on_key_press(event):
    key_press_handler(event, drawing_area, toolbar)

def _quit():
    root.quit()
    root.destroy()

def file_open_handler():
    print("LOG: Opening file " + selected_file.get())
    print("LOG: DT:s " +  str(csv_handler.find_DT_numbers(selected_file.get())) + " found.")
    DT_numbers = []
    DT_numbers = csv_handler.find_DT_numbers(selected_file.get())
    data_list = database_handler.get_DT_list(DT_numbers)
    print("LOG: Datalist: " + str(data_list))
    draw_checkboxes(data_list) #Also, delete old checkboxes for file changing

def draw_checkboxes(list):
    checkboxes.clear()
    datalist_vars.clear()
    for i, item in enumerate(list):
        var = tkinter.IntVar()
        datalist_vars.append(var)
        checkbox = tkinter.Checkbutton(master=checkbox_frame, text = item, variable=var, bg='white', anchor='w')
        checkboxes.append(checkbox)
        checkbox.grid(row=i, column=0, sticky='W')
    #print(datalist_vars[0].get())

def draw_selected():
    list = get_draw_list()
    print(list)

def get_draw_list():
    draw_list = []
    for i in range(0, len(datalist_vars)):
        if datalist_vars[i].get() == 1:
            draw_list.append(i)
    #print(draw_list)
    return draw_list

textfield_filelist = tkinter.Label(master=root, text="Select file:")
textfield_filelist.grid(row=0, column=9, rowspan=1, columnspan=1)

selected_file = tkinter.StringVar()
file_selection = ttk.Combobox(master=root, width=50, textvariable=selected_file)
file_selection['values'] = filelist
file_selection.grid(row=1, column=9, rowspan=1, columnspan=1) #pack(side=tkinter.RIGHT)

open_button = tkinter.Button(master=root, text="Open file", command=file_open_handler)
open_button.grid(row=2, column=9, rowspan=1, columnspan=1)

quit_button = tkinter.Button(master=root, text="Quit", command=_quit)
quit_button.grid(row=20, column=9, rowspan=1, columnspan=1) #pack(side=tkinter.RIGHT)

checkbox_frame = tkinter.Frame(root, bg="white", highlightbackground="black", highlightthickness=1, width=300, height=800)
checkbox_frame.grid(row=5, column=9, rowspan=5, columnspan=1)

draw_button = tkinter.Button(master=root, text="Draw", command=draw_selected)
draw_button.grid(row=11, column=9, rowspan=1, columnspan=1)

tkinter.mainloop()