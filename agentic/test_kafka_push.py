#!/usr/bin/env python3
"""Test script to push messages to Kafka and verify via UI."""

import time
import requests
import json
from kafka import KafkaProducer
from kafka.errors import KafkaError

def test_kafka_push():
    # Wait for Kafka to be ready
    print("Waiting for Kafka to be ready...")
    for i in range(30):
        try:
            result = subprocess.run(
                ["docker", "exec", "kafka", "kafka-topics", "--bootstrap-server", "kafka:29092", "--list"],
                capture_output=True, text=True, timeout=10
            )
            if result.stdout.strip():
                print(f"Kafka ready after {i+1} seconds")
                break
        except:
            time.sleep(1)
    
    # Connect to Kafka using internal IP
    print("Connecting to Kafka...")
    producer = KafkaProducer(
        bootstrap_servers=['172.20.0.3:29092'],
        value_serializer=lambda v: json.dumps(v).encode('utf-8'),
        key_serializer=lambda k: json.dumps(k).encode('utf-8')
    )
    
    # Send test messages
    print("Sending test messages...")
    topics = ['mytopic', 'analytics']
    for topic in topics:
        producer.send(topic, value={"test": "message", "timestamp": time.time()})
        producer.flush()
        print(f"Sent messages to {topic}")
    
    # Verify messages
    print("Verifying messages via Kafka UI API...")
    try:
        response = requests.get('http://localhost:8081/api/clusters/local/topics')
        topics_response = response.json()
        print(f"Topics from UI: {topics_response}")
        
        # Try to get message count for mytopic
        if 'mytopic' in topics_response.get('topics', []):
            print("✓ mytopic found in Kafka UI")
        else:
            print("✗ mytopic NOT found in Kafka UI")
            
    except Exception as e:
        print(f"Error fetching topics: {e}")
    
    # Clean up
    producer.flush()
    producer.close()
    producer.disconnect()
    print("Test completed!")

if __name__ == "__main__":
    import subprocess
    test_kafka_push()
