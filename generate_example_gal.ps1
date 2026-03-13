$particles = @(
    @(0.10, 0.20, 1.00,  0.010,  0.000, 1.0),
    @(0.40, 0.30, 1.20, -0.005,  0.007, 0.8),
    @(0.70, 0.80, 0.90,  0.000, -0.006, 0.6),
    @(0.25, 0.65, 1.10,  0.004, -0.002, 0.9),
    @(0.85, 0.15, 0.95, -0.003,  0.005, 0.7)
)

$outFile = Join-Path (Get-Location) "example_input_5.gal"
$fs = [System.IO.File]::Open($outFile, [System.IO.FileMode]::Create)

try {
    foreach ($particle in $particles) {
        foreach ($value in $particle) {
            $bytes = [System.BitConverter]::GetBytes([double]$value)
            $fs.Write($bytes, 0, $bytes.Length)
        }
    }
}
finally {
    $fs.Close()
}

Write-Host "Wrote $outFile"
Write-Host "Particle count: $($particles.Count)"
Write-Host "File size bytes: $((Get-Item $outFile).Length)"