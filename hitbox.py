import tkinter as tk
from tkinter import filedialog
from PIL import Image, ImageTk, ImageDraw
import numpy as np
from skimage import measure
from shapely.geometry import Polygon, MultiPolygon
from shapely.ops import unary_union

class HitboxApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Hitbox Finder")

        # Setup canvas for sprite display
        self.canvas = tk.Canvas(root, width=1000, height=600)
        self.canvas.pack()

        # Frame to hold the load button and tolerance scale side by side
        self.top_frame = tk.Frame(root)
        self.top_frame.pack(pady=10)

        # Button for loading the image
        self.load_button = tk.Button(self.top_frame, text="Charger Sprite", command=self.load_image)
        self.load_button.pack(side=tk.LEFT, padx=10)

        # Tolerance scale
        self.tolerance_label = tk.Label(self.top_frame, text="Tolerance (0.0 - 1000.0):")
        self.tolerance_label.pack(side=tk.LEFT)
        self.tolerance_scale = tk.Scale(self.top_frame, from_=0.0, to_=1000.0, resolution=1.0, orient=tk.HORIZONTAL, length=600, sliderlength=30, command=self.update_tolerance)
        self.tolerance_scale.set(50)  # Default tolerance value
        self.tolerance_scale.pack(side=tk.LEFT)

        # Label to display normalized cursor position
        self.cursor_pos_label = tk.Label(root, text="Normalized Position: X=0.000, Y=0.000")
        self.cursor_pos_label.pack()

        # Frame to hold the listbox and its buttons
        self.list_frame = tk.Frame(root)
        self.list_frame.pack(pady=10)

        # Scrollable listbox to display point coordinates
        self.listbox_frame = tk.Frame(self.list_frame)
        self.listbox_frame.pack(side=tk.LEFT)

        self.scrollbar = tk.Scrollbar(self.listbox_frame)
        self.scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        self.point_listbox = tk.Listbox(self.listbox_frame, selectmode=tk.SINGLE, yscrollcommand=self.scrollbar.set, width=30, height=10)
        self.point_listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self.scrollbar.config(command=self.point_listbox.yview)

        # Frame to hold the listbox manipulation buttons
        self.button_frame = tk.Frame(self.list_frame)
        self.button_frame.pack(side=tk.LEFT, padx=10)

        # Buttons to manipulate the listbox items
        self.move_up_button = tk.Button(self.button_frame, text="Move Up", command=self.move_point_up)
        self.move_up_button.pack(fill=tk.X)

        self.move_down_button = tk.Button(self.button_frame, text="Move Down", command=self.move_point_down)
        self.move_down_button.pack(fill=tk.X)

        self.delete_point_button = tk.Button(self.button_frame, text="Delete Point", command=self.delete_point)
        self.delete_point_button.pack(fill=tk.X)

        # Button to add hitbox points to the list
        self.add_hitbox_points_button = tk.Button(self.button_frame, text="Add Hitbox Points", command=self.add_hitbox_points_to_list)
        self.add_hitbox_points_button.pack(fill=tk.X)

        # Button to print coordinates in the terminal
        self.print_points_button = tk.Button(self.button_frame, text="Print Coordinates", command=self.print_coordinates)
        self.print_points_button.pack(fill=tk.X)

        self.image = None
        self.sprite = None
        self.original_image = None
        self.modified_image = None
        self.scale_factor = None
        self.tolerance = 0.5  # Default tolerance value
        self.hitbox_points = []  # Store the points of the current hitbox

    def load_image(self):
        file_path = filedialog.askopenfilename()
        if file_path:
            self.original_image = Image.open(file_path).convert("RGBA")
            self.scale_factor = 1  # Scale factor used for resizing the image
            self.display_image(self.original_image)
            # Bind the mouse motion event to track cursor position
            self.canvas.bind("<Motion>", self.update_cursor_position)
            # Bind mouse click to add point to list
            self.canvas.bind("<Button-1>", self.add_point_on_click)
            self.process_image()

    def display_image(self, image):
        # Convert the image to a format suitable for Tkinter display
        self.sprite = ImageTk.PhotoImage(image)
        self.canvas.create_image(0, 0, anchor=tk.NW, image=self.sprite)

    def update_tolerance(self, value):
        self.tolerance = float(value)
        self.process_image()

    def process_image(self):
        if self.original_image:
            # Create a copy of the original image to modify
            self.modified_image = self.original_image.copy()

            # Convert image to numpy array for processing
            img_array = np.array(self.original_image)

            # Create a mask for non-transparent areas
            mask = img_array[:, :, 3] > 0

            # Find contours in the mask
            contours = measure.find_contours(mask, 0.5)

            # Convert contours to polygons
            polygons = []
            for contour in contours:
                contour = np.flip(contour, axis=1)  # Flip to (x, y)
                polygons.append(Polygon(contour))

            # Merge polygons and simplify contours using the current tolerance
            multi_polygon = unary_union(polygons)
            simplified_polygon = multi_polygon.simplify(self.tolerance, preserve_topology=False)

            # Draw the simplified hitbox on the modified image
            self.draw_hitbox_on_image(simplified_polygon)

    def draw_hitbox_on_image(self, simplified_polygon):
        if self.modified_image:
            # Create a drawing object on the modified image
            draw = ImageDraw.Draw(self.modified_image)

            # Check if the simplified_polygon is a MultiPolygon
            if isinstance(simplified_polygon, MultiPolygon):
                simplified_polygon = max(simplified_polygon, key=lambda p: p.area)  # Pick largest polygon

            if isinstance(simplified_polygon, Polygon):
                self.hitbox_points = np.array(simplified_polygon.exterior.coords)
                print(*self.hitbox_points, sep=" ")

                # Convert points to match original image size
                if self.scale_factor:
                    points = [(x * self.scale_factor, y * self.scale_factor) for x, y in self.hitbox_points]

                # Draw the polygon (hitbox) on the modified image
                draw.polygon(points, outline="red", fill=None)

                # Display the updated image with hitbox
                self.display_image(self.modified_image)
            else:
                print("No valid polygon found.")

    def update_cursor_position(self, event):
        if self.original_image:
            img_width, img_height = self.original_image.size

            # Get the cursor's position in pixels
            x, y = event.x, event.y

            # Normalize the coordinates to be in the range [0, 1]
            x_normalized = x / img_width
            y_normalized = y / img_height

            # Update the label with the normalized position
            self.cursor_pos_label.config(text=f"Normalized Position: X={x_normalized:.3f}, Y={y_normalized:.3f}")

    def add_point_on_click(self, event):
        if self.original_image:
            img_width, img_height = self.original_image.size

            # Get the cursor's position in pixels
            x, y = event.x, event.y

            # Normalize the coordinates
            x_normalized = x / img_width
            y_normalized = y / img_height

            # Add the point to the listbox
            self.point_listbox.insert(tk.END, f"X={x_normalized}, Y={y_normalized}")

    def add_hitbox_points_to_list(self):
        width, height = self.modified_image.size
        if self.hitbox_points is not None:
            for point in self.hitbox_points:
                x, y = point
                self.point_listbox.insert(tk.END, f"X={x/width}, Y={y/height}")

    def move_point_up(self):
        selected = self.point_listbox.curselection()
        if selected:
            index = selected[0]
            if index > 0:
                # Swap the selected item with the one above it
                point = self.point_listbox.get(index)
                self.point_listbox.delete(index)
                self.point_listbox.insert(index - 1, point)
                self.point_listbox.selection_set(index - 1)

    def move_point_down(self):
        selected = self.point_listbox.curselection()
        if selected:
            index = selected[0]
            if index < self.point_listbox.size() - 1:
                # Swap the selected item with the one below it
                point = self.point_listbox.get(index)
                self.point_listbox.delete(index)
                self.point_listbox.insert(index + 1, point)
                self.point_listbox.selection_set(index + 1)

    def delete_point(self):
        selected = self.point_listbox.curselection()
        if selected:
            index = selected[0]
            self.point_listbox.delete(index)

    def print_coordinates(self):
        points = self.point_listbox.get(0, tk.END)
        points = [p.replace("X=", "").replace("Y=", "").replace(",", "") for p in points]
        points_str = " ".join(points)
        print(points_str)

if __name__ == "__main__":
    root = tk.Tk()
    app = HitboxApp(root)
    root.mainloop()
