intentos_permitidos = 3
pin_guardado = 1234

if __name__ == "__main__":
    intentos_realizados = 0
    acceso_concedido = False

    while intentos_realizados < intentos_permitidos and not acceso_concedido:
        try:
            pin_ingresado = int(input("Introduce pin: "))
            if pin_ingresado == pin_guardado:
                acceso_concedido = True
            else:
                intentos_realizados += 1
                print("Pin incorrecto")
        except ValueError:
            print("Solo puedes ingresar numeros")
    resultado = "ACCESO" if acceso_concedido else "DENEGADO"
    print(resultado)
