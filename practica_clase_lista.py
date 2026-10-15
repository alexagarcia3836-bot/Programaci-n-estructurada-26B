import random
import time

def suma_objetivo(lista, objetivo):
    n = len(lista)
    for i in range(n):
  
        for j in range(i + 1, n): 
            
            if lista[i] + lista[j] == objetivo:
                return True
    return False

if __name__ == "__main__":

    tamaños = [1000, 2000, 4000, 8000]

    for n in tamaños:

        lista = [random.randint(1, 10000) for _ in range(n)]

        objetivo_imposible = -1

        inicio = time.perf_counter()

        suma_objetivo(lista, objetivo_imposible)

        final = time.perf_counter()

        tiempo_ejecucion = final - inicio
        print(f"Para n = {n:<4} | Tiempo de ejecucion: {tiempo_ejecucion:.6f} segundos")
