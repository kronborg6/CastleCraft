import argparse
import hashlib
import json
import subprocess
import urllib.request
from pathlib import Path

MANIFEST_URL = "https://piston-meta.mojang.com/mc/game/" "version_manifest_v2.json"

ROOT = Path(__file__).resolve().parent.parent
CACHE = ROOT / ".cache" / "minecraft"
DATA = ROOT / "generated"


def read_json(url):
    with urllib.request.urlopen(url, timeout=30) as response:
        return json.load(response)


def generate(version):

    output = DATA / version
    blocks = output / "reports" / "blocks.json"

    if blocks.exists():
        print(f"Minecraft {version} data already exists")
        return
    manifest = read_json(MANIFEST_URL)

    entry = next((v for v in manifest["versions"] if v["id"] == version), None)

    if entry is None:
        raise RuntimeError(f"Unknown Minecraft version: {version}")

    metadata = read_json(entry["url"])
    server = metadata["downloads"]["server"]

    jar_dir = CACHE / version
    jar_dir.mkdir(parents=True, exist_ok=True)

    jar_path = jar_dir / "server.jar"

    if not jar_path.exists():
        print(f"Downloading Minecraft {version}...")
        urllib.request.urlretrieve(server["url"], jar_path)

    actual_hash = hashlib.sha1(jar_path.read_bytes()).hexdigest()

    if actual_hash != server["sha1"]:
        jar_path.unlink()
        raise RuntimeError("Server JAR SHA-1 verification failed")

    output = DATA / version
    blocks = output / "reports" / "blocks.json"

    if blocks.exists():
        print("Minecraft data already generated")
        return

    output.mkdir(parents=True, exist_ok=True)

    print(f"Generating Minecraft {version} reports...")

    subprocess.run(
        [
            "java",
            "-DbundlerMainClass=net.minecraft.data.Main",
            "-jar",
            str(jar_path),
            "--reports",
            "--output",
            str(output),
        ],
        cwd=jar_dir,
        check=True,
    )

    if not blocks.exists():
        raise RuntimeError("blocks.json was not generated")

    print(f"Generated data in {output}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("version")
    args = parser.parse_args()

    generate(args.version)
