import csv
import unicodedata

def normalizar(texto):
    return ''.join(
        c for c in unicodedata.normalize('NFD', texto)
        if unicodedata.category(c) != 'Mn'
    )

def burbuja(lista, clave):
    n = len(lista)
    for i in range(n-1):
        for j in range(n-1-i):
            if lista[j][clave] > lista[j+1][clave]:
                
                lista[j], lista[j+1] = lista[j+1], lista[j]
            if normalizar(lista[j][clave]) > normalizar(lista[j+1][clave]):
                lista[j], lista[j+1] = lista[j+1], lista[j]
    return lista

def contenido_csv(nombre_archivo):
    with open(nombre_archivo, mode='r', newline='', encoding='utf-8') as archivo:
        lector = csv.DictReader(archivo)
        pacientes = list(lector)

    pacientes_ordenados = burbuja(pacientes, "nombre")
    return pacientes_ordenados

    print("Pacientes ordenados por nombre:")
    for p in pacientes_ordenados:
        print(f" {p['id']},{p['apellido']},{p['nombre']},{p['edad']},{p['anio_nacimiento']},{p['fecha_ingreso']},{p['estudio']},{p['prioridad']},{p['temperatura']},{p['frecuencia_cardiaca']},{p['saturacion']},{p['espera_min']}")

def escribir_csv(nombre_archivo, datos, columnas):
    # Abrimos el archivo en modo escritura
    with open(nombre_archivo, mode='w', newline='', encoding='utf-8') as archivo:
        escritor = csv.DictWriter(archivo, fieldnames=columnas)
        escritor.writeheader()   # Escribimos los encabezados
        escritor.writerows(datos)  # Escribimos todas las filas

if __name__ == "__main__":
    ruta = r"C:\Users\carlos\Documents\Programas\Practica_clase_listas\pacientes.csv"
    ruta_escribir = r"C:\Users\carlos\Documents\Programas\Practica_clase_listas\pacientes_ordenados.csv"

    pacientes_ordenados = contenido_csv(ruta)
    columnas = pacientes_ordenados[0].keys()  # Obtener los nombres de las columnas del primer diccionario
    escribir_csv(ruta_escribir, pacientes_ordenados, columnas)

    print(f"Archivo ordenado guardado en: {ruta_escribir}")