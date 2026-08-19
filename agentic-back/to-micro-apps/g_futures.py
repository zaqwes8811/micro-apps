import requests

from typing import List, Dict, Any

def get_futures_contracts(settle: str = 'usdt') -> List[Dict[str, Any]]:
    url = f"https://api.gateio.ws/api/v4/futures/{settle}/contracts"
    headers = {
        'Content-Type': 'application/json'
    }
    
    response = requests.get(url, headers=headers)
    
    if response.status_code == 200:
        return response.json()
    else:
        return f"Failed to retrieve futures contracts: {response.status_code}"

if __name__ == '__main__':
    print(get_futures_contracts())
