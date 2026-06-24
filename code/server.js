const express = require("express");
const fetch = (...args) => import("node-fetch").then(({ default: fetch }) => fetch(...args));

const app = express();
const PORT = 3000;

/* CHANGE THIS → your ESP32 IP */
const ESP32_URL = "http://172.18.48.187/data";

/* Serve frontend */
app.use(express.static(__dirname));

/* API route */
app.get("/data", async (req, res) => {
    try {
        const controller = new AbortController();
        const timeout = setTimeout(() => controller.abort(), 2000); // ⏱️ 2 sec timeout

        const response = await fetch(ESP32_URL, {
            signal: controller.signal
        });

        clearTimeout(timeout);

        if (!response.ok) {
            throw new Error(`ESP32 HTTP error: ${response.status}`);
        }

        const data = await response.json();

        console.log("✅ Data received from ESP32");

        res.json(data);

    } catch (err) {
        console.error("❌ ESP32 Error:", err.message);

        res.status(500).json({
            error: "ESP32 not reachable",
            details: err.message
        });
    }
});

/* Start server */
app.listen(PORT, () => {
    console.log(`🚀 Server running at http://localhost:${PORT}`);
});