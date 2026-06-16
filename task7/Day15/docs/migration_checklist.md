# Migration Checklist: Mosquitto Lab → Zelbytes Production MQTTS

> Complete this checklist when Zelbytes endpoints go live on `careers.zelbytes.com`.  
> The firmware uses `#ifdef ZELBYTES_PLATFORM` guards, so migration is a **config change — not a code rewrite**.

---

## Pre-Migration (Do Before Endpoint Goes Live)

- [ ] Confirm token issuance is open on the careers portal
- [ ] Read the official endpoint announcement on `careers.zelbytes.com`
- [ ] Note your assigned `DEVICE_TOKEN` from the portal — store it securely, not in source code
- [ ] Verify your device's firmware includes `zelbytes_config.h` and compiles cleanly with `ZELBYTES_PLATFORM` **undefined** (lab mode)

---

## Step 1 — Obtain Credentials

- [ ] Log in to `careers.zelbytes.com`
- [ ] Navigate to the Device / Token management section
- [ ] Generate or copy your `DEVICE_TOKEN` for each physical device
- [ ] Save tokens in a secure location (password manager, environment variable, or secrets file not tracked by git)

---

## Step 2 — Update Build Configuration

- [ ] Add `-D ZELBYTES_PLATFORM` to your build flags:
  - **PlatformIO:** add `build_flags = -D ZELBYTES_PLATFORM` to `platformio.ini`
  - **Arduino IDE:** add `-DZELBYTES_PLATFORM` in *File → Preferences → Additional compiler flags*
  - **CMake:** add `target_compile_definitions(firmware PRIVATE ZELBYTES_PLATFORM)`
- [ ] Pass `DEVICE_TOKEN` via build flags (do **not** hardcode):
  ```
  -D DEVICE_TOKEN=\"your_actual_token_here\"
  ```
- [ ] Verify `zelbytes_config.h` picks up the new values — compile and check for the `DEVICE_TOKEN placeholder` warning (it should **not** appear)

---

## Step 3 — Network / TLS Configuration

- [ ] Confirm device has internet access (not just LAN)
- [ ] Ensure port **8883** (MQTTS) is open on your router / firewall
- [ ] Ensure port **443** (HTTPS) is open for HTTP ingest
- [ ] Obtain and flash the Zelbytes CA certificate to the device (if required — check portal docs)
- [ ] Set `WiFiClientSecure` (or equivalent) to use TLS — do not use `setInsecure()` in production

---

## Step 4 — MQTT Migration

| Setting | Lab Value | Production Value |
|---|---|---|
| Host | `localhost` / local IP | `careers.zelbytes.com` |
| Port | `1883` | `8883` |
| TLS | Off | On |
| Username | (none) | (check portal — may be device ID) |
| Password | (none) | `DEVICE_TOKEN` |
| Client ID | `labdev-<suffix>` | `zeldev-<suffix>` (or portal-assigned) |

- [ ] Confirm MQTT CONNECT succeeds — check serial output for `CONNACK` with return code `0`
- [ ] Publish a test message to `iot/telemetry` and verify it appears in the dashboard

---

## Step 5 — HTTP Ingest Migration

| Setting | Lab Value | Production Value |
|---|---|---|
| URL | `http://localhost:8080/telemetry` | `https://careers.zelbytes.com/api/v1/iot/telemetry` |
| Auth header | (none) | `Authorization: Bearer <DEVICE_TOKEN>` |
| TLS | Off | On |

- [ ] Send a test POST with a valid `SensorBundle v1` payload
- [ ] Confirm HTTP `200 OK` (or `201 Created`) response
- [ ] Verify payload appears in the Zelbytes dashboard

---

## Step 6 — Validation

- [ ] Run device for at least **10 minutes** in production mode — check dashboard for continuous data
- [ ] Confirm timestamp values are correct (timezone / epoch conversion)
- [ ] Confirm all sensor fields appear in the dashboard with expected values
- [ ] Check `battery_mv` and `rssi_dbm` are nested correctly under `device` in the API response
- [ ] Verify no reconnect loops in serial output

---

## Step 7 — Cleanup

- [ ] Remove any hardcoded lab IP addresses from source code
- [ ] Add `DEVICE_TOKEN` to `.gitignore` or your secrets exclusion list
- [ ] Update `README.md` to reflect production endpoints
- [ ] Archive Mosquitto lab config (don't delete — useful for offline testing)
- [ ] Mark this checklist complete and notify mentor for phase review

---

## Rollback Plan

If production is unstable, revert to lab mode by removing `-D ZELBYTES_PLATFORM` from build flags and recompiling. No firmware logic changes required.
