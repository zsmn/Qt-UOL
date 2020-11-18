
"""Script for Tkinter GUI chat client."""
from socket import AF_INET, socket, SOCK_STREAM
from threading import Thread
import tkinter


def receive():
    """Handles receiving of messages."""
    while True:
        try:
            msg = client_socket.recv(1024).decode("utf8")
            msg_list.insert(tkinter.END, msg)
        except OSError:  # Possibly client has left the chat.
            break


def send(event=None):  # event is passed by binders.
    """Handles sending of messages."""
    msg = my_msg.get()
    my_msg.set("")  # Clears input field.
    client_socket.send(bytes(msg, "utf8"))
    if msg == "{quit}":
        client_socket.close()
        top.quit()


def onClosing(event=None):
    """This function is to be called when the window is closed."""
    my_msg.set("{quit}")
    send()

top = tkinter.Tk()
top.title("Chatter")

messages_frame = tkinter.Frame(top)
my_msg = tkinter.StringVar()  # For the messages to be sent.
my_name = tkinter.StringVar()  # For the messages to be sent.
my_msg.set("Type your messages here.")
scrollbar = tkinter.Scrollbar(messages_frame)  # To navigate through past messages.
# Following will contain the messages.
msg_list = tkinter.Listbox(messages_frame, height=30, width=100, yscrollcommand=scrollbar.set)
scrollbar.pack(side=tkinter.RIGHT, fill=tkinter.Y)
msg_list.pack(side=tkinter.LEFT, fill=tkinter.BOTH)
msg_list.pack()
messages_frame.pack()

listbox = tkinter.Listbox(top)
listbox.pack()

listbox.insert(tkinter.END, "a list entry")

entry_field = tkinter.Entry(top, textvariable=my_msg)
name_field = tkinter.Entry(top, textvariable=my_name)

entry_field.bind("<Return>", send)
entry_field.pack()
name_field.pack()
send_button = tkinter.Button(top, text="Send", command=send)
send_button.pack()

top.protocol("WM_DELETE_WINDOW", onClosing)

#----Now comes the sockets part----
host = '127.0.0.1'
port = 33002

BUFSIZ = 1024

client_socket = socket(AF_INET, SOCK_STREAM)
client_socket.connect((host, port))

receive_thread = Thread(target=receive)
receive_thread.start()
tkinter.mainloop()  # Starts GUI execution.

