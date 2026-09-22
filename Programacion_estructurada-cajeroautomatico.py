multiplo_valido = 50
limite_diario = 6000.0

def retiro_cajero():
    saldo = float(input("Saldo Actual: "))
    retirado_hoy = float(input("Monto acumulado hoy: "))
    monto = float(input("Monto a Retirar: "))

    if monto % multiplo_valido != 0:
        mensaje = "Monto no valido"
    elif monto > saldo:
        mensaje = "Saldo Insuficiente"
    elif (retirado_hoy + monto) > limite_diario:
        mensaje = "Limite Diario Excedido"
    else:
        saldo = saldo - monto
        mensaje = "Entregado"

    
    print(f"Resultado: {mensaje} | saldo final: ${saldo:.2f}")


    retiro_cajero()