import random
import time
import matplotlib.pyplot as plt

def suma_objetivo(lista, objetivo):
    n = len(lista)
    for i in range(n):
        for i in range(i + 1, n):
            if lista[i] + lista[j] == objetivo:
                return True
    return False

if __name__ == "__main__":
    lista = [1,2,3,4,5]

    lista1 = [i for i in range (10000)]

    objetivo = 9
    resultado = suma_objetivo(lista, objetivo)

    inicio = time.perf_counter()

    final = time.perf_counter()

    print(f"tiempo de ejecucion: {final - inicio: 6f} segundos ")


