import tkinter as tk
from tkinter import filedialog
from PIL import Image, ImageTk

def get_normalized_position(event, img_width, img_height):
    # Get the cursor's position in pixels
    x, y = event.x, event.y

    # Normalize the coordinates to be in the range [0, 1]
    x_normalized = x / img_width
    y_normalized = y / img_height

    # Update the label with the normalized position
    label_var.set(f"Normalized Position: X={x_normalized:.3f}, Y={y_normalized:.3f}")

def choose_image():
    # Open a file dialog to choose an image
    file_path = filedialog.askopenfilename(filetypes=[("Image files", "*.jpg;*.jpeg;*.png;*.bmp;*.gif")])
    if file_path:
        display_image(file_path)

def display_image(image_path):
    # Load the image using PIL
    img = Image.open(image_path)
    img_width, img_height = img.size

    # Convert the image to a format that Tkinter can use
    img_tk = ImageTk.PhotoImage(img)

    # Update the canvas size and display the image
    canvas.config(width=img_width, height=img_height)
    canvas.create_image(0, 0, anchor=tk.NW, image=img_tk)
    canvas.image = img_tk  # Keep a reference to prevent garbage collection

    # Bind the mouse movement event to update the cursor position
    canvas.bind("<Motion>", lambda event: get_normalized_position(event, img_width, img_height))

def main():
    global canvas, label_var

    # Create the main window
    root = tk.Tk()
    root.title("Image Hover Cursor Position")

    # Create a button to choose an image
    btn_choose_image = tk.Button(root, text="Choose Image", command=choose_image)
    btn_choose_image.pack()

    # Create a label to display the normalized position
    label_var = tk.StringVar()
    label = tk.Label(root, textvariable=label_var)
    label.pack()

    # Create a canvas where the image will be displayed
    canvas = tk.Canvas(root)
    canvas.pack()

    # Run the Tkinter main loop
    root.mainloop()

if __name__ == "__main__":
    main()
