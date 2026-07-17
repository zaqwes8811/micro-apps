#!/usr/bin/env python3
import time
import os
from kafka import KafkaProducer

# Get Kafka internal IP from Docker
KAFKA_IP = os.environ.get('KAFKA_INTERNAL_IP', '172.21.0.3')
KAFKA_PORT = os.environ.get('KAFKA_PORT', '29092')

# Create producer with auto.create.topic enabled
producer = KafkaProducer(
    bootstrap_servers=f'{KAFKA_IP}:{KAFKA_PORT}',
    value_serializer=lambda v: str(v).encode('utf-8'),
    acks=1,
    retries=3,
    request_timeout_ms=60000
)

print("Sending test messages...")

try:
    # Topics should already exist, just send messages
    for i in range(3):
        producer.send('mytopic', value=f'Test message {i}')
        producer.send('analytics', value=f'Analytics data {i}')
        producer.flush()
        print(f"Message {i} sent successfully!")
    
    producer.close()
    print("\nAll messages sent successfully!")
except Exception as e:
    print(f"Error: {e}")
    import traceback
    traceback.print_exc()
    producer.close()
