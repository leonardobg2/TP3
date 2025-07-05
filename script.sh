#!/usr/bin/bash

DATA_GENERATOR_BIN="./bin/exp.out"
SIMULATOR_BIN="./bin/tp2.out"
RESULTS_FILE="resultados_simulacao.csv" # Nome do arquivo CSV para os resultados

# Limpa o arquivo de resultados e adiciona o cabeçalho CSV
> "$RESULTS_FILE"
echo "NumNodes,NumPackets,RearmCount,ExecutionTimeSeconds" >> "$RESULTS_FILE"

# Intervalos para o número de NÓS (armazéns)
# Ajuste esses valores conforme a capacidade do seu computador e o tempo que você tem.
# Exemplo: de 10 em 10 até 100
for NUM_NODES in $(seq 10 250 999); do
    # Intervalos para o número de PACOTES
    # Ajuste esses valores para não sobrecarregar
    for NUM_PACKETS in $(seq 100 100 1000); do # Exemplo: de 100 em 100 até 1000

        echo "Rodando simulação para N_NODES=${NUM_NODES}, N_PACKETS=${NUM_PACKETS}"

        CURRENT_INPUT_FILE="exp_input_${NUM_NODES}_${NUM_PACKETS}.txt"

        # Gerar dados de entrada
        if ! "$DATA_GENERATOR_BIN" "${NUM_NODES}" "${NUM_PACKETS}" > "${CURRENT_INPUT_FILE}"; then
            echo "ERRO: Falha ao gerar dados para ${NUM_NODES} nós e ${NUM_PACKETS} pacotes."
            exit 1
        fi

        # Rodar o simulador e capturar a saída
        # A saída do seu main.cpp é: RearmCount\nExecutionTimeSeconds
        # Precisamos capturar ambas as linhas
        SIM_OUTPUT=$("$SIMULATOR_BIN" "${CURRENT_INPUT_FILE}")

        # Extrair RearmCount e ExecutionTimeSeconds
        REARM_COUNT=$(echo "$SIM_OUTPUT" | head -n 1)
        EXEC_TIME=$(echo "$SIM_OUTPUT" | tail -n 1 | sed 's/ segundos//') # Remove " segundos"

        # Adicionar os resultados ao arquivo CSV
        echo "${NUM_NODES},${NUM_PACKETS},${REARM_COUNT},${EXEC_TIME}" >> "$RESULTS_FILE"

        # Remover o arquivo de entrada temporário
        rm "${CURRENT_INPUT_FILE}"

    done
done

echo "Simulação concluída. Resultados salvos em $RESULTS_FILE"