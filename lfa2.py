
class Automato:
    def __init__(self, estados, alfabeto, transicoes, inicial, finais):
        self.estados = estados
        self.alfabeto = alfabeto
        self.transicoes = transicoes  # {(origem, destino): expressão}
        self.inicial = inicial
        self.finais = finais

    def adicionar_transicao(self, origem, destino, simbolo):
        chave = (origem, destino)
        if chave in self.transicoes:
            self.transicoes[chave] = f"({self.transicoes[chave]}|{simbolo})"
        else:
            self.transicoes[chave] = simbolo

    def eliminar_estado(self, estado):
        entradas = [(p, estado) for p in self.estados if (p, estado) in self.transicoes]
        saidas = [(estado, q) for q in self.estados if (estado, q) in self.transicoes]
        loop = self.transicoes.get((estado, estado), "")

        for (p, _), r1 in [(e, self.transicoes[e]) for e in entradas]:
            for (_, q), r2 in [(s, self.transicoes[s]) for s in saidas]:
                nova_expr = f"{r1}({loop})*{r2}" if loop else f"{r1}{r2}"
                self.adicionar_transicao(p, q, nova_expr)

        # Remover transições relacionadas ao estado eliminado
        for chave in list(self.transicoes):
            if estado in chave:
                del self.transicoes[chave]
        self.estados.remove(estado)

    def converter_para_expressao_regular(self):
        estados_aux = self.estados.copy()
        while len(estados_aux) > 2:
            for estado in estados_aux:
                if estado != self.inicial and estado not in self.finais:
                    self.eliminar_estado(estado)
                    break
            estados_aux = self.estados.copy()

        return self.transicoes.get((self.inicial, list(self.finais)[0]), "")



def menu_interativo():
    print("=== Conversor de AFD para Expressão Regular ===")

    # Estados
    estados = set(input("Digite os estados separados por espaço (ex: q0 q1 q2): ").split())

    # Alfabeto
    alfabeto = set(input("Digite os símbolos do alfabeto separados por espaço (ex: a b): ").split())

    # Estado inicial
    inicial = input("Digite o estado inicial: ")

    # Estados finais
    finais = set(input("Digite os estados finais separados por espaço: ").split())

    # Transições
    print("\nDigite as transições no formato: origem simbolo destino")
    print("Digite 'fim' para encerrar as transições.")
    transicoes = {}
    while True:
        entrada = input("Transição: ")
        if entrada.lower() == "fim":
            break
        try:
            origem, simbolo, destino = entrada.split()
            chave = (origem, destino)
            if chave in transicoes:
                transicoes[chave] = f"({transicoes[chave]}|{simbolo})"
            else:
                transicoes[chave] = simbolo
        except ValueError:
            print("Formato inválido. Use: origem simbolo destino")

    # Criação e conversão
    afd = Automato(estados, alfabeto, transicoes, inicial, finais)
    regex = afd.converter_para_expressao_regular()
    print("\nExpressão regular equivalente ao AFD:")
    print(regex)


# Executa o menu
if __name__ == "__main__":
    menu_interativo()