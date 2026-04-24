from flask import Flask, render_template, request, jsonify
from flask_cors import CORS
from crop_data import CROPS_DATA, MONTH_CROPS, FARMER_RECOMMEND
from datetime import datetime

app = Flask(__name__)
CORS(app)

# ─── HOME PAGE ────────────────────────────────────────
@app.route('/')
def home():
    return render_template('index.html')

# ─── FARMER MODE PAGE ─────────────────────────────────
@app.route('/farmer')
def farmer():
    return render_template('farmer_mode.html')

# ─── ADVANCED MODE PAGE ───────────────────────────────
@app.route('/advanced')
def advanced():
    return render_template('advanced_mode.html')

# ─── CROP ANALYSIS PAGE ───────────────────────────────
@app.route('/analysis')
def analysis():
    return render_template('crop_analysis.html')

# ─── CROP CALENDAR PAGE ───────────────────────────────
@app.route('/calendar')
def calendar():
    return render_template('crop_calendar.html')

# ─── OUTPUT PAGE ──────────────────────────────────────
@app.route('/output')
def output():
    return render_template('output.html')

# ─── API: GET ALL CROPS ───────────────────────────────
@app.route('/api/crops')
def get_all_crops():
    return jsonify({
        'status': 'success',
        'total': len(CROPS_DATA),
        'crops': CROPS_DATA
    })

# ─── API: GET SINGLE CROP ─────────────────────────────
@app.route('/api/crop/<crop_key>')
def get_crop(crop_key):
    crop = CROPS_DATA.get(crop_key)
    if not crop:
        return jsonify({'status': 'error', 'message': 'Crop not found'}), 404
    return jsonify(crop)

# ─── API: FARMER MODE RECOMMENDATION ─────────────────
@app.route('/api/recommend/farmer')
def recommend_farmer():
    soil = request.args.get('soil', '').lower()
    rainfall = request.args.get('rainfall', '').lower()
    water = request.args.get('water', '').lower()
    season = request.args.get('season', '').lower()

    recommendation = FARMER_RECOMMEND.get(soil, {
        'best': 'rice',
        'top3': ['rice', 'maize', 'groundnut']
    })

    best_key = recommendation['best']
    top3_keys = recommendation['top3']

    best_crop = CROPS_DATA.get(best_key, {})
    top3_crops = []
    for key in top3_keys:
        crop = CROPS_DATA.get(key)
        if crop:
            top3_crops.append(crop)

    return jsonify({
        'status': 'success',
        'mode': 'farmer',
        'inputs': {
            'soil': soil,
            'rainfall': rainfall,
            'water': water,
            'season': season
        },
        'best': best_crop,
        'top3': top3_crops
    })

# ─── API: ADVANCED MODE RECOMMENDATION ───────────────
@app.route('/api/recommend/advanced')
def recommend_advanced():
    try:
        n = float(request.args.get('n', 0))
        p = float(request.args.get('p', 0))
        k = float(request.args.get('k', 0))
        temp = float(request.args.get('temp', 25))
        ph = float(request.args.get('ph', 6.5))
        rainfall = float(request.args.get('rainfall', 800))
    except ValueError:
        return jsonify({'status': 'error', 'message': 'Invalid input values'}), 400

    # Scoring logic
    scores = {}
    for key, crop in CROPS_DATA.items():
        score = 50

        # Rainfall scoring
        if rainfall > 1000:
            if crop['water'] == 'High':
                score += 30
            elif crop['water'] == 'Medium':
                score += 15
        elif rainfall > 500:
            if crop['water'] == 'Medium':
                score += 30
            elif crop['water'] == 'High':
                score += 10
            elif crop['water'] == 'Low':
                score += 15
        else:
            if crop['water'] == 'Low':
                score += 30
            elif crop['water'] == 'Low–Medium':
                score += 25
            elif crop['water'] == 'Medium':
                score += 10

        # pH scoring
        try:
            ph_range = crop['ph'].replace('–', '-').split('-')
            ph_min = float(ph_range[0].strip())
            ph_max = float(ph_range[1].strip())
            if ph_min <= ph <= ph_max:
                score += 25
            elif abs(ph - ph_min) < 0.5 or abs(ph - ph_max) < 0.5:
                score += 10
        except:
            score += 10

        # Temperature scoring
        try:
            temp_range = crop['temp'].replace('°C', '').replace('–', '-').split('-')
            temp_min = float(temp_range[0].strip())
            temp_max = float(temp_range[1].strip())
            if temp_min <= temp <= temp_max:
                score += 25
            elif abs(temp - temp_min) < 3 or abs(temp - temp_max) < 3:
                score += 10
        except:
            score += 10

        # NPK scoring
        if n > 80:
            if key in ['rice', 'maize', 'sugarcane', 'banana']:
                score += 15
        if p > 40:
            if key in ['turmeric', 'potato', 'tomato', 'onion']:
                score += 15
        if k > 40:
            if key in ['banana', 'potato', 'tomato', 'sugarcane']:
                score += 15

        scores[key] = score

    # Sort by score
    sorted_crops = sorted(scores.items(), key=lambda x: x[1], reverse=True)
    top3_keys = [k for k, v in sorted_crops[:3]]

    best_crop = CROPS_DATA.get(top3_keys[0], {})
    top3_crops = [CROPS_DATA[k] for k in top3_keys if k in CROPS_DATA]

    return jsonify({
        'status': 'success',
        'mode': 'advanced',
        'inputs': {
            'n': n, 'p': p, 'k': k,
            'temp': temp, 'ph': ph,
            'rainfall': rainfall
        },
        'best': best_crop,
        'top3': top3_crops
    })

# ─── API: CROP CALENDAR ───────────────────────────────
@app.route('/api/calendar')
def get_crop_calendar():
    try:
        month = int(request.args.get('month', datetime.now().month - 1))
        month = max(0, min(11, month))
    except ValueError:
        month = datetime.now().month - 1

    crops = MONTH_CROPS.get(month, [])
    crops_with_details = []
    for crop_name, score in crops:
        key = crop_name.lower().replace(' ', '')
        crop_info = CROPS_DATA.get(key, {})
        crops_with_details.append({
            'name': crop_name,
            'score': score,
            'desc': crop_info.get('desc', ''),
            'tamil_name': crop_info.get('tamil_name', ''),
            'season': crop_info.get('season', '')
        })

    return jsonify({
        'status': 'success',
        'month': month,
        'crops': crops_with_details
    })

# ─── API: CURRENT MONTH ───────────────────────────────
@app.route('/api/current_month')
def current_month():
    return jsonify({'month': datetime.now().month - 1})

# ─── ERROR HANDLERS ───────────────────────────────────
@app.errorhandler(404)
def not_found(error):
    return jsonify({'status': 'error', 'message': 'Page not found'}), 404

@app.errorhandler(500)
def server_error(error):
    return jsonify({'status': 'error', 'message': 'Server error'}), 500

# ─── RUN APP ──────────────────────────────────────────
if __name__ == '__main__':
    import os
    port = int(os.environ.get("PORT", 5000))
    app.run(debug=False, host="0.0.0.0", port=port)