import numpy as np
import matplotlib.pyplot as plt

project_root = Path(__file__).resolve().parent.parent
csv_path = project_root / "out" / "build" / "x64-Debug" / "rd_map.csv"
image_path = project_root / "docs" / "radar_heatmap.png"

data = np.loadtxt(csv_path, delimiter=",")


plt.imshow(data, origin="lower", aspect="auto")
plt.xlabel("Doppler Bin")
plt.ylabel("Range Bin")
plt.title("Synthetic Range-Doppler Map")
plt.colorbar()
plt.tight_layout()
plt.savefig(image_path, dpi=150)
plt.show()