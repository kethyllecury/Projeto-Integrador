from fastapi import FastAPI, Header, HTTPException
from pydantic import BaseModel, Field
from pymongo import MongoClient
from typing import Optional
from datetime import datetime
from dotenv import load_dotenv
import os

load_dotenv()

API_KEY = os.getenv("API_KEY")
MONGO_URI = os.getenv("MONGO_URI")
DB_NAME = os.getenv("DB_NAME", "meubanco")
COLLECTION_NAME = os.getenv("COLLECTION_NAME", "api")


client = MongoClient(MONGO_URI)
db = client[DB_NAME]
collection = db[COLLECTION_NAME]

app = FastAPI(title="RFID Backend com MongoDB Atlas")


class RfidPayload(BaseModel):
    uid: str = Field(..., description="UID do cartão RFID")
    tag_type: Optional[str] = None
    rssi: Optional[int] = None
    timestamp: Optional[str] = None
    extra: Optional[dict] = None


def verify_api_key(x_api_key: Optional[str]):
    if x_api_key != API_KEY:
        raise HTTPException(status_code=401, detail="Invalid API Key")

@app.post("/api/rfid", status_code=201)
async def receive_rfid(payload: RfidPayload, x_api_key: Optional[str] = Header(None)):
    verify_api_key(x_api_key)

    doc = payload.dict()
    doc["timestamp"] = payload.timestamp or datetime.utcnow().isoformat()
    doc["received_at"] = datetime.utcnow().isoformat()

    result = collection.insert_one(doc)

    return {"status": "ok", "id": str(result.inserted_id), "uid": payload.uid}

@app.get("/api/readings/latest")
async def latest(n: int = 10, x_api_key: Optional[str] = Header(None)):
    verify_api_key(x_api_key)

    docs = list(collection.find().sort("_id", -1).limit(n))
    for d in docs:
        d["_id"] = str(d["_id"])  
    return docs
