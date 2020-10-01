"""
This simple Python program takes a table of LED positions and generates the `led_config_t` struct
`g_led_config` for inclusion in the keyboard's `config.h`.
"""
import csv, sys, os

FILENAME = "led_matrix_config.csv"

NO_LED = "NO_LED"

KEY_TO_LED = [
    [ 12, 11, 10,  9,  8,  7 ],
    [ 13, 14, 15, 16, 17, 18 ],
    [ 24, 23, 22, 21, 20, 19 ],
    [ 25, 26, 27, 28, 29, 30 ],
    [ NO_LED, 35, 24, 33, 32, 21 ],
]

def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

def write_key_to_led(layout):
    """
    Write the first element of the struct.
    """
    for row in layout:
        sys.stdout.write("    { ")
        formatted = map(lambda led: str(led).rjust(2), row)
        sys.stdout.write(", ".join(formatted))
        sys.stdout.write(" },\n")

def main():
    """
    Main program.
    """
    # Output the key to LED struct
    sys.stdout.write("led_config_t g_led_config = { {\n")

    def _add_if_led(led):
        try:
            return led + 35
        except TypeError:
            return led

    write_key_to_led(KEY_TO_LED)
    # Now write the right half
    write_key_to_led(map(lambda row: map(_add_if_led, reversed(row)), KEY_TO_LED))

    sys.stdout.write("}, {\n")

    leds = []
    min_x = min_y = max_x = max_y = None

    filename = os.path.join(os.path.dirname(__file__), FILENAME)
    with open(filename) as fp:
        reader = csv.reader(fp)
        next(reader)
        for index, pos_x, pos_y, flag in reader:
            index = int(index)
            pos_x = int(pos_x)
            pos_y = int(pos_y)
            if min_x is None:
                min_x = max_x = pos_x
                min_y = max_y = pos_y
            else:
                min_x = min(min_x, pos_x)
                max_x = max(max_x, pos_x)
                min_y = min(min_y, pos_y)
                max_y = max(max_y, pos_y)
            leds.append((index, pos_x, pos_y, flag))

    # Python2 is stupid
    max_x = float(max_x)
    max_y = float(max_y)
    
    def _write_positions(mirror=False):
        for row in chunks(leds, 6):
            sys.stdout.write("    ")
            for _, pos_x, pos_y, flag in row:
                val_x = round((pos_x - min_x) / (max_x - min_x) * 112)
                if mirror:
                    val_x = 224 - val_x
                val_y = round((pos_y - min_y) / (max_y - min_y) * 64)

                sys.stdout.write("{ %3d, %3d }, " % (val_x, val_y))
            sys.stdout.write("\n")
    
    # Left half first
    _write_positions(False)
    # Slightly adjusted right half - the LED positions are mirrored
    _write_positions(True)

    sys.stdout.write("}, {\n")
    
    for _ in range(2):
        # Write out the flags twice, for each side
        for row in chunks(leds, 6):
            sys.stdout.write("    ")
            for _, _, _, flag in row:
                sys.stdout.write("%s, " % (flag,))
            sys.stdout.write("\n")

    sys.stdout.write("} };\n")



if __name__ == "__main__":
    main()
