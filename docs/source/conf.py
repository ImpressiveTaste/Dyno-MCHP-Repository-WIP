import os
import sys
sys.path.insert(0, os.path.abspath('../..'))

project = 'MC-Dyno'
author = 'Marco Trevisan / Microchip Technology'
release = '0.1.0'

extensions = [
    'sphinx.ext.napoleon',
    'sphinx.ext.viewcode',
]

templates_path = ['_templates']
exclude_patterns = [
    'hardware.rst',
    'modes.rst',
    'x2c_scope.rst',
]

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
html_css_files = ['custom.css']

# Optional: logo (file may be added later)
html_logo = '_static/images/mc_dyno_logo_big.png'
html_theme_options = {
    'logo_only': True,
}
