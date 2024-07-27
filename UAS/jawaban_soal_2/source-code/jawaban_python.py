try:
   hasil = 10/0
except ZeroDivisionError:
   print("Error: Pembagian oleh Nol tidak diizinkan")
else:
   print("Hasil Pembagian:", hasil)
finally:
   print("Selesai")