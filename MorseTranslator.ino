import tkinter as tk
from tkinter import messagebox
import RPi.GPIO as GPIO
from time import sleep

##setup board and LED
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)

##initialising the board
LED = 8
BUZZER = 10
GPIO.setup(LED, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(BUZZER, GPIO.OUT, initial=GPIO.LOW)
##dict for getting character translations
MORSE_CODE_DICT = { 'A':'.-', 'B':'-...',
                    'C':'-.-.', 'D':'-..', 'E':'.',
                    'F':'..-.', 'G':'--.', 'H':'....',
                    'I':'..', 'J':'.---', 'K':'-.-',
                    'L':'.-..', 'M':'--', 'N':'-.',
                    'O':'---', 'P':'.--.', 'Q':'--.-',
                    'R':'.-.', 'S':'...', 'T':'-',
                    'U':'..-', 'V':'...-', 'W':'.--',
                    'X':'-..-', 'Y':'-.--', 'Z':'--..',
                    '1':'.----', '2':'..---', '3':'...--',
                    '4':'....-', '5':'.....', '6':'-....',
                    '7':'--...', '8':'---..', '9':'----.',
                    '0':'-----', ', ':'--..--', '.':'.-.-.-',
                    '?':'..--..', '/':'-..-.', '-':'-....-',
                    '(':'-.--.', ')':'-.--.-'}
##setup the window  
root=tk.Tk()
root.geometry("800x600")
root.title("Morse Code Translator")  
message_var=tk.StringVar()

#function to blink the led
def blinkLED(led, buzzer,cipher):
    GPIO.output(led, GPIO.LOW)
    for c in cipher:
        ##one time unit for a dot
        if c == '.':
            GPIO.output(led, GPIO.LOW)
            GPIO.output(buzzer, GPIO.LOW)
            sleep(.15)
            GPIO.output(led, GPIO.HIGH)
            GPIO.output(buzzer, GPIO.HIGH)
            sleep(.15)
        ##two time units for a dash
        elif c == '-':
            GPIO.output(led, GPIO.LOW)
            GPIO.output(buzzer, GPIO.LOW)
            sleep(.15)
            GPIO.output(led, GPIO.HIGH)
            GPIO.output(buzzer, GPIO.HIGH)
            sleep(0.45)
        ##two units for a whitespace
        else:
            GPIO.output(led, GPIO.LOW)
            GPIO.output(buzzer, GPIO.LOW)
            sleep(0.3)
##function to convert the input message into morse code
def encrypt(message):
    ##initialise empty string to return
    cipher = ''
    ##iterate through message and construct the morse code cipher
    for letter in message:
        if letter != ' ':
              cipher += MORSE_CODE_DICT[letter]
        else:
            ##add an additional white space to allow for telling difference
            ##between character and word spacing
            cipher += '  '
 
    return cipher

##function to run required functionality
def ToMorse(message):
    cipher = encrypt(message)
    blinkLED(LED, BUZZER, cipher)      
    GPIO.output(BUZZER, GPIO.LOW)
    GPIO.output(LED, GPIO.LOW)

##function ran on pressing submit button
def submit():
    ##store the message locally
    message = message_var.get()
    message = message.upper()
    ##translate to morse if the length is correct
    if(len(message) <= 12):
        ToMorse(message)
    ##otherwise show error message and clear input
    else:
        messagebox.showerror("Message too long","Message must be 12 characters or less")
        message_var.set("")
##input message label    
message_label = tk.Label(root, text = 'Input message of 12 characters or less: ', font=('calibre',10, 'bold'))
##storing the message
message_entry = tk.Entry(root,textvariable = message_var, font=('calibre',10,'normal'))
##submit button
sub_btn=tk.Button(root,text = 'Submit', command = submit)
##page layout info
message_label.grid(row=0,column=0)
message_entry.grid(row=0,column=1)
sub_btn.grid(row=2,column=1)

root.mainloop()
