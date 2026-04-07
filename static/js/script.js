// ─── LANGUAGE SYSTEM ─────────────────────────────────
const TRANSLATIONS = {
    en: {
        'hero-title': 'Choose Your Mode',
        'hero-sub': 'Select the best mode for your farming needs',
        'lbl-farmer': 'Farmer Mode',
        'lbl-advanced': 'Advanced Mode',
        'lbl-analysis': 'Crop Analysis',
        'lbl-calendar': 'Crop Calendar',
        'desc-farmer': 'Simple dropdown inputs for quick recommendations',
        'desc-advanced': 'Enter NPK, pH, temperature for precise results',
        'desc-analysis': 'Analyze detailed information about any crop',
        'desc-calendar': 'Monthly bar graph of recommended crops',
    },
    ta: {
        'hero-title': 'உங்கள் முறையை தேர்வு செய்யுங்கள்',
        'hero-sub': 'உங்கள் விவசாய தேவைக்கு சிறந்த முறையை தேர்வு செய்யுங்கள்',
        'lbl-farmer': 'விவசாயி முறை',
        'lbl-advanced': 'மேம்பட்ட முறை',
        'lbl-analysis': 'பயிர் பகுப்பாய்வு',
        'lbl-calendar': 'பயிர் நாட்காட்டி',
        'desc-farmer': 'விரைவான பரிந்துரைக்கு எளிய தேர்வுகள்',
        'desc-advanced': 'NPK, pH, வெப்பநிலை உள்ளிட்டு துல்லியமான முடிவுகள்',
        'desc-analysis': 'எந்த பயிரையும் பற்றிய விரிவான தகவல்கள்',
        'desc-calendar': 'மாதாந்திர பரிந்துரைக்கப்பட்ட பயிர்களின் வரைப்படம்',
    }
};

function setLang(lang) {
    const translations = TRANSLATIONS[lang];
    for (const [id, text] of Object.entries(translations)) {
        const el = document.getElementById(id);
        if (el) el.textContent = text;
    }
    document.getElementById('btn-en')?.classList.toggle('active', lang === 'en');
    document.getElementById('btn-ta')?.classList.toggle('active', lang === 'ta');
    localStorage.setItem('lang', lang);
}

// ─── RESTORE LANGUAGE ON PAGE LOAD ───────────────────
document.addEventListener('DOMContentLoaded', () => {
    const savedLang = localStorage.getItem('lang') || 'en';
    if (document.getElementById('btn-en')) {
        setLang(savedLang);
    }
});

// ─── LOADING ANIMATION ────────────────────────────────
function showLoading() {
    const existing = document.getElementById('loading-overlay');
    if (existing) return;
    const overlay = document.createElement('div');
    overlay.id = 'loading-overlay';
    overlay.style.cssText = `
        position: fixed; top: 0; left: 0;
        width: 100%; height: 100%;
        background: rgba(26,61,31,0.85);
        display: flex; flex-direction: column;
        align-items: center; justify-content: center;
        z-index: 9999;
    `;
    overlay.innerHTML = `
        <div style="font-size:48px;margin-bottom:16px">🌾</div>
        <div style="color:#9FE1CB;font-size:18px;font-family:Poppins,sans-serif;font-weight:500">
            Finding best crops...
        </div>
        <div style="color:#5DCAA5;font-size:14px;margin-top:8px;font-family:Poppins,sans-serif">
            பயிர்களை தேடுகிறோம்...
        </div>
    `;
    document.body.appendChild(overlay);
}

function hideLoading() {
    const overlay = document.getElementById('loading-overlay');
    if (overlay) overlay.remove();
}

// ─── FORM VALIDATION ──────────────────────────────────
function validateForm(fields) {
    for (const field of fields) {
        const el = document.getElementById(field);
        if (!el || !el.value) {
            el?.style.setProperty('border-color', '#e24b4a');
            setTimeout(() => el?.style.setProperty('border-color', '#e0e0e0'), 2000);
            return false;
        }
    }
    return true;
}

// ─── ANIMATE BARS ON CALENDAR PAGE ───────────────────
function animateBars() {
    const fills = document.querySelectorAll('.bar-fill');
    fills.forEach((fill, i) => {
        const targetWidth = fill.style.width;
        fill.style.width = '0%';
        setTimeout(() => {
            fill.style.width = targetWidth;
        }, i * 120);
    });
}

// ─── NUMBER COUNTER ANIMATION ────────────────────────
function animateCounters() {
    const counters = document.querySelectorAll('.stat-box h3');
    counters.forEach(counter => {
        const target = parseInt(counter.textContent);
        if (isNaN(target)) return;
        let current = 0;
        const step = Math.ceil(target / 30);
        const timer = setInterval(() => {
            current += step;
            if (current >= target) {
                current = target;
                clearInterval(timer);
            }
            counter.textContent = current + (counter.textContent.includes('+') ? '+' : '');
        }, 40);
    });
}

// ─── SCROLL ANIMATIONS ────────────────────────────────
function initScrollAnimations() {
    const cards = document.querySelectorAll('.mode-card, .form-card, .output-card');
    const observer = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.style.opacity = '1';
                entry.target.style.transform = 'translateY(0)';
            }
        });
    }, { threshold: 0.1 });

    cards.forEach(card => {
        card.style.opacity = '0';
        card.style.transform = 'translateY(20px)';
        card.style.transition = 'opacity 0.5s ease, transform 0.5s ease';
        observer.observe(card);
    });
}

// ─── INIT ON PAGE LOAD ────────────────────────────────
document.addEventListener('DOMContentLoaded', () => {
    initScrollAnimations();
    animateCounters();

    // Animate bars if on calendar page
    if (document.getElementById('bar-chart')) {
        setTimeout(animateBars, 500);
    }
});