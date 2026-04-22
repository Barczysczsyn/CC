import re

def adiciona_inicio_fim(estados, transicoes, start, finals):
    initial_estado = 'START'
    final_estado = 'END'
    
    # Adiciona os estados inicial e final à lista de estados
    updated_estados = [initial_estado] + [s for s in estados if s] + [final_estado]
    # Inicializa as transições do estado inicial
    updated_transicoes = {initial_estado: {'ε': start}}
    
    for estado in transicoes:
        if estado:
            updated_transicoes[estado] = {simbolo: alvo for simbolo, alvo in transicoes[estado].items() if alvo}
            
            if estado in finals:
                updated_transicoes[estado]['ε'] = final_estado
    
    updated_transicoes[final_estado] = {}
    
    return updated_estados, updated_transicoes, initial_estado, final_estado

def simplifica_expressao(expression):
    # Remove pares de parênteses vazios
    while '()' in expression:
        expression = expression.replace('()', '')

    # Substitui o símbolo & por ε
    expression = expression.replace('(&)', 'ε')
    # Remove espaços desnecessários nos parênteses
    expression = re.sub(r'\(\s*([^\(\)]+)\s*\)', r'\1', expression)
    # Substitui múltiplos & por um único &
    expression = re.sub(r'&\+?', '&', expression)
    # Remove & das concatenações
    expression = expression.replace('&', '')

    return expression

def converte_afn_para_afd(estados, transicoes, start, finals, alfabeto):
    start_afd_estado = frozenset([start])
    afd_estados = [start_afd_estado]
    afd_transicoes = {}
    afd_estados_finais = set()
    to_process = [start_afd_estado]

    while to_process:
        current_afd_estado = to_process.pop()
        current_transicoes = {}

        for simbolo in alfabeto:
            proximo_estados = set()

            for nfa_estado in current_afd_estado:
                if simbolo in transicoes.get(nfa_estado, {}):
                    proximo_estados.update(transicoes[nfa_estado][simbolo])

            if proximo_estados:
                next_afd_estado = frozenset(proximo_estados)

                if next_afd_estado not in afd_estados:
                    afd_estados.append(next_afd_estado)
                    to_process.append(next_afd_estado)

                current_transicoes[simbolo] = next_afd_estado

        if current_transicoes:
            afd_transicoes[current_afd_estado] = current_transicoes

        if any(estado in finals for estado in current_afd_estado):
            afd_estados_finais.add(current_afd_estado)

    estado_map = {}
    for index, estado in enumerate(afd_estados):
        estado_map[estado] = f'q{index}'

    afd_estados_str = [estado_map[estado] for estado in afd_estados]
    afd_transicoes_str = {
        estado_map[estado]: {
            simbolo: estado_map[proximo_estado]
            for simbolo, proximo_estado in transicoes.items()
        }
        for estado, transicoes in afd_transicoes.items()
    }
    afd_estado_inicial_str = estado_map[start_afd_estado]
    afd_estados_finais_str = {estado_map[estado] for estado in afd_estados_finais}

    return afd_estados_str, afd_transicoes_str, afd_estado_inicial_str, list(afd_estados_finais_str)

def afd_para_regex(estados_list, transicoes_dict, initial_estado, estados_finais_set):
    augmented_estados, augmented_transicoes, start_marker, end_marker = \
        adiciona_inicio_fim(estados_list, transicoes_dict, initial_estado, estados_finais_set)

    while len(augmented_estados) > 2:  # Enquanto houver mais de dois estados (inicial e final)
        temp_estado = augmented_estados[1]  # Seleciona um estado temporário para remoçao

        for fonte in augmented_estados:
            if fonte == temp_estado:
                continue  # 'fonte' nao pode ser o estado temporário

            epsilon = ''
            # Encontrar transições de 'fonte' para o estado temporário
            for simbolo, alvo in augmented_transicoes.get(fonte, {}).items():
                if alvo == temp_estado:
                    epsilon = simbolo  # Transiçao de 'fonte' para 'temp_estado' encontrada

            if not epsilon:
                continue  # Sem transiçao de 'fonte' para o temporário, continue

            loop_temporario = []
            # Encontrar loops no estado temporário
            for loop_simbolo, loop_alvo in augmented_transicoes.get(temp_estado, {}).items():
                if loop_alvo == temp_estado:
                    loop_temporario.append(loop_simbolo)

            # Criar expressao regular para os loops do estado temporário
            loop = '|'.join(loop_temporario) if len(loop_temporario) > 1 else (loop_temporario[0] if loop_temporario else '')

            # Processar transições do estado temporário para outros estados
            for simbolo_transicao, proximo_estado in augmented_transicoes.get(temp_estado, {}).items():
                if proximo_estado == temp_estado:
                    continue  # 'proximo_estado' nao pode ser o temporário

                if not simbolo_transicao:
                    continue  # Sem transiçao para 'proximo_estado', continue

                # Tratar transições epsilon (ε) representadas por '&'
                if epsilon == '&':
                    epsilon = ''  # Ignorar ε se for para concatenaçao
                if simbolo_transicao == '&':
                    simbolo_transicao = ''  # Ignorar ε se for para concatenaçao

                # Construir o novo padrao de expressao regular
                if not loop:
                    novo_padrao = f'{epsilon}{simbolo_transicao}'
                else:
                    novo_padrao = f'{epsilon}({loop})*{simbolo_transicao}'

                # Verificar e combinar transicoes existentes
                merged = False
                for simbolo, existing_alvo in augmented_transicoes.get(fonte, {}).items():
                    if existing_alvo == proximo_estado:
                        combined_pattern = f'({novo_padrao})|({simbolo})'
                        augmented_transicoes[fonte][combined_pattern] = proximo_estado
                        augmented_transicoes[fonte].pop(simbolo, None)  # Remove a transiçao antiga
                        merged = True
                        break

                if not merged:
                    augmented_transicoes[fonte][novo_padrao] = proximo_estado

        augmented_estados.remove(temp_estado)  # Remover o estado temporário
        augmented_transicoes.pop(temp_estado, None)

    # Construir o padrao final a partir do estado inicial e final
    regex_final = ''
    for simbolo, alvo in augmented_transicoes[start_marker].items():
        if alvo == end_marker:
            regex_final = simbolo

    # Ajustar para eliminar & no padraoo final
    regex_final = re.sub(r'&', '', regex_final)  # Remove & em concatenaçao e apenas no final

    return simplifica_expressao(regex_final)

def get_input(prompt, valores_validos=None, permitir_vazio=False):
    while True:
        user_input = input(prompt).strip()
        if permitir_vazio and user_input == '':
            return []
        if valores_validos:
            valores = [value.strip() for value in user_input.split(',')]
            if all(value in valores_validos for value in valores):
                return valores
            else:
                print(f"Valor digitado invalido. Valores permitidos: {', '.join(valores_validos)}")
        else:
            return [value.strip() for value in user_input.split(',')]

def main():
    while True:
        estados = get_input("Digite os estados separados por virgula: ")
        estado_inicial = get_input("Digite o estado inicial: ", estados)[0]
        estados_finais = get_input("Digite os estados finais separados por virgula: ", estados)
        alfabeto = get_input("Digite o alfabeto separado por virgula: ")
        transicoes = {}
        print("\nEnter the transicoes for each estado:")
        for estado in estados:
            transicoes[estado] = {}
            for simbolo in alfabeto + ['&']:
                destido = get_input(f"Delta({estado}, {simbolo}): ", estados, permitir_vazio=True)
                if destido:
                    transicoes[estado][simbolo] = destido
        
        afd_estados, afd_transicoes, afd_estado_inicial, afd_estados_finais = converte_afn_para_afd(estados, transicoes, estado_inicial, estados_finais, alfabeto)
        regex = afd_para_regex(afd_estados, afd_transicoes, afd_estado_inicial, set(afd_estados_finais))

        print("\nExpressaon Regular final: ")
        print(regex)

        if input("\nDeseja inserir um novo automato? (s/n): ").strip().lower() != 's':
            print("\nSaindo do programa...\n")
            break

if __name__ == "__main__":
    main()