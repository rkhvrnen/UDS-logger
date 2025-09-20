import tkinter
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
from matplotlib.backend_bases import key_press_handler
from matplotlib.figure import Figure
import numpy as np

root = tkinter.Tk()
root.wm_title("Data")

fig = Figure(figsize=(5, 10), dpi=100)
ax = fig.add_subplot(1, 1, 1)
x = np.linspace(0, 10, 100)
y = np.sin(x)
ax.plot(x, y)
ax.grid()
#fig.add_axes(ax)

drawing_area = FigureCanvasTkAgg(fig, master=root)
drawing_area.draw()
#drawing_area.get_tk_widget().pack(side=tkinter.LEFT, fill=tkinter.BOTH, expand=1)

toolbar = NavigationToolbar2Tk(drawing_area, root)
toolbar.update()
drawing_area.get_tk_widget().pack(side=tkinter.LEFT, fill=tkinter.BOTH, expand=1)

def on_key_press(event):
    key_press_handler(event, drawing_area, toolbar)

def _quit():
    root.quit()
    root.destroy()

button = tkinter.Button(master=root, text="Quit", command=_quit)
button.pack(side=tkinter.TOP)

tkinter.mainloop()