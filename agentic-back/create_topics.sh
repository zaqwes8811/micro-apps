#!/bin/bash
set -e

# Get Kafka internal IP
KAFKA_IP="${KAFKA_INTERNAL_IP:-172.21.0.3}"
KAFKA_PORT="${KAFKA_PORT:-29092}"

echo "Creating topics via kafka-topics.sh..."
echo "Using: ${KAFKA_IP}:${KAFKA_PORT}"

# Create topics (binaries are in /usr/bin/ inside container)
docker exec -t kafka /usr/bin/kafka-topics.sh \
    --bootstrap-server "${KAFKA_IP}:${KAFKA_PORT}" \
    --create \
    --topic mytopic \
    --partitions 1 \
    --replication-factor 1

docker exec -t kafka /usr/bin/kafka-topics.sh \
    --bootstrap-server "${KAFKA_IP}:${KAFKA_PORT}" \
    --create \
    --topic analytics \
    --partitions 1 \
    --replication-factor 1

echo "Topics created!"
