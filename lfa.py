#converter afn para afd
from collections import defaultdict

def eh_afn(estados, transicoes):
    transicoes_por_estado_simbolo = defaultdict(set)
    for (origem, destino), simbolo in transicoes.items():
        transicoes_por_estado_simbolo[(origem, simbolo)].add(destino)
        if simbolo == "&":
            return True  # ε-transição detectada
    for destinos in transicoes_por_estado_simbolo.values():
        if len(destinos) > 1:
            return True  # múltiplos destinos para mesmo símbolo
    return False

def converter_afn_para_afd(estados, alfabeto, transicoes, inicial, finais):
    def epsilon_closure(conjunto):
        pilha = list(conjunto)
        closure = set(conjunto)
        while pilha:
            estado = pilha.pop()
            for (origem, destino), simbolo in transicoes.items():
                if origem == estado and simbolo == "" and destino not in closure:
                    closure.add(destino)
                    pilha.append(destino)
        return closure

    def mover(conjunto, simbolo):
        destinos = set()
        for estado in conjunto:
            for (origem, destino), s in transicoes.items():
                if origem == estado and s == simbolo:
                    destinos.add(destino)
        return destinos

    estado_inicial_afd = frozenset(epsilon_closure({inicial}))
    fila = [estado_inicial_afd]
    visitados = set()
    novo_transicoes = {}
    novo_estados = set()
    novo_finais = set()

    while fila:
        atual = fila.pop()
        if atual in visitados:
            continue
        visitados.add(atual)
        novo_estados.add(atual)

        if any(e in finais for e in atual):
            novo_finais.add(atual)

        for simbolo in alfabeto:
            destino = epsilon_closure(mover(atual, simbolo))
            if destino:
                destino_frozen = frozenset(destino)
                novo_transicoes[(atual, destino_frozen)] = simbolo
                fila.append(destino_frozen)

    # Renomear estados para strings
    nome_estados = {estado: f"S{idx}" for idx, estado in enumerate(novo_estados)}
    transicoes_renomeadas = {}
    for (origem, destino), simbolo in novo_transicoes.items():
        transicoes_renomeadas[(nome_estados[origem], nome_estados[destino])] = simbolo

    return (
        set(nome_estados.values()),
        alfabeto,
        transicoes_renomeadas,
        nome_estados[estado_inicial_afd],
        {nome_estados[e] for e in novo_finais}
    )
#converter afn para afd

# 🔧 Exemplo de uso
'''
estados = {"q0", "q1", "q2", "q3"}
alfabeto = {"a", "b"}
transicoes = {
    ("q0", "q1"): "a",
    ("q1", "q2"): "b",
    ("q2", "q3"): "a",
    ("q3", "q3"): "b"
}
inicial = "q0"
finais = {"q2", "q3"}

afd = Automato(estados, alfabeto, transicoes, inicial, finais)
regex = afd.converter_para_expressao_regular()
print("Expressão regular equivalente:", regex)
'''


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

    if eh_afn(estados, transicoes):
        print("\n⚠️ O autômato inserido é NÃO determinístico (AFN). Convertendo para AFD...")
        estados, alfabeto, transicoes, inicial, finais = converter_afn_para_afd(
            estados, alfabeto, transicoes, inicial, finais
        )

    afd = Automato(estados, alfabeto, transicoes, inicial, finais)
    regex = afd.converter_para_expressao_regular()
    print("\nExpressão regular equivalente ao AFD:")
    print(regex)


    # Criação e conversão
    afd = Automato(estados, alfabeto, transicoes, inicial, finais)
    regex = afd.converter_para_expressao_regular()
    print("\nExpressão regular equivalente ao AFD:")
    print(regex)

class Automato:
    def __init__(self, estados, alfabeto, transicoes, inicial, finais):
        self.estados = estados.copy()
        self.alfabeto = alfabeto
        self.transicoes = transicoes.copy()
        self.inicial = inicial
        self.finais = finais.copy()

        # Adiciona estado final único
        self.estado_final_unico = "F"
        self.estados.add(self.estado_final_unico)
        for f in self.finais:
            self.adicionar_transicao(f, self.estado_final_unico, "")  # ε-transição
        self.finais = {self.estado_final_unico}

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

        for chave in list(self.transicoes):
            if estado in chave:
                del self.transicoes[chave]
        self.estados.remove(estado)

    def converter_para_expressao_regular(self):
        estados_aux = self.estados.copy()
        while len(estados_aux) > 2:
            for estado in estados_aux:
                if estado != self.inicial and estado != self.estado_final_unico:
                    self.eliminar_estado(estado)
                    break
            estados_aux = self.estados.copy()

        return self.transicoes.get((self.inicial, self.estado_final_unico), "")

# Executa o menu
if __name__ == "__main__":
    menu_interativo()
    


