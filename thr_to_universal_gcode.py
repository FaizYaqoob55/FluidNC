import os
import glob
import math

def convert_thr_to_universal_gcode():
    print("=== .thr to G-Code Converter ===")
    
    # Get all .thr files in the current folder
    thr_files = glob.glob("*.thr")
    
    if len(thr_files) == 0:
        print("Koi .thr file nahi mili is folder mein!")
        input("Press Enter to exit...")
        return
        
    print(f"Total {len(thr_files)} .thr files mili hain.")
    
    # Table ka size yahan set karein! (Center se kinare tak)
    TABLE_RADIUS_MM = 125.0 
    
    print(f"Table Radius Set: {TABLE_RADIUS_MM} mm")
    print("Converting...\n")
    
    for file in thr_files:
        gcode_filename = file.replace(".thr", ".gcode")
        
        with open(file, 'r') as thr, open(gcode_filename, 'w') as gcode:
            # Add some starting setup commands
            gcode.write("; Converted for Sand Table\n")
            gcode.write("G90 ; Absolute positioning\n")
            gcode.write("G21 ; Millimeters\n")
            gcode.write("F2000 ; Speed\n\n")
            
            for line in thr:
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                
                parts = line.split()
                if len(parts) >= 2:
                    try:
                        theta_radians = float(parts[0])
                        radius_normalized = float(parts[1])
                        
                        # Math Calculation for FluidNC
                        theta_degrees = theta_radians * (180.0 / math.pi)
                        radius_mm = radius_normalized * TABLE_RADIUS_MM
                        
                        gcode.write(f"G1 X{theta_degrees:.3f} Y{radius_mm:.3f}\n")
                    except ValueError:
                        pass
                    
        print(f"✅ Converted: {file} -> {gcode_filename}")
        
    print("\nSab files convert ho gayi hain!")
    print("Ab ye Gcode files upload karein, apka design 100% theek banega!")
    input("Press Enter to exit...")

if __name__ == "__main__":
    convert_thr_to_universal_gcode()
