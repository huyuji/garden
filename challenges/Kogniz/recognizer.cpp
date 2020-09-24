#include <memory>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>

using namespace std;

class Person {};
class PersonInfo {};

class RecognizerModel {
public:
	RecognizerModel() {};
	~RecognizerModel() {};

	void train_with_latest_data() {};

	Person identify_person(PersonInfo info) {};
};

class Recognizer {
public:
	Recognizer():
		m_isHighLoad(false),
		m_retrainRunning(true),
		m_model(make_shared<RecognizerModel>())
	{
		m_model->train_with_latest_data();
		m_retrainThread = make_unique<thread>(&Recognizer::retrain_thread, this);
	};

	~Recognizer() {
		m_retrainRunning = false;
		m_needRetrain = false;
		m_retrainCV.notify_all();
		m_retrainThread->join();
	};

	void queue_retrain() {
		{
			lock_guard<mutex> lock(m_retrainMutex);
			m_needRetrain = true;
		}
		m_retrainCV.notify_one();
	};

	Person identify_person(PersonInfo info) {
		auto model = get_model();
		return model->identify_person(info);
	};

	void turnOnHighLoad() {
		m_isHighLoad = true;
	}

	void turnOffHighLoad() {
		m_isHighLoad = false;
	}

private:
	/*
	* Non-blocking lock will be used where suitable under high load.
	* Otherwise spin lock will be used for faster response.
	*/
	bool m_isHighLoad;

	unique_ptr<thread> m_retrainThread;
	bool m_retrainRunning;

	mutex m_retrainMutex;
	condition_variable m_retrainCV;
	bool m_needRetrain;

	/*
	* Make a copy of this shared_ptr before use to ensure the old model is still available after it is replaced by the new model.
	* Synchronize access to this shared_ptr to ensure thread safety.
	*/
	shared_ptr<RecognizerModel> m_model;
	shared_mutex m_modelMutex;

	void retrain_thread() {
		while (m_retrainRunning) {
			unique_lock<mutex> lock(m_retrainMutex);
			m_retrainCV.wait(lock);

			if (m_retrainRunning && m_needRetrain) {
				m_needRetrain = false;
				lock.unlock();
				retrain();
			}
		}
	};

	void retrain() {
		auto newModel = make_shared<RecognizerModel>();
		newModel->train_with_latest_data();
		set_model(newModel);
	}

	shared_ptr<RecognizerModel> get_model() {
		if (m_isHighLoad) {
			shared_lock<shared_mutex> lock(m_modelMutex);
			auto model = m_model;
			return model;
		}
		else {
			return atomic_load(&m_model);
		}
	}

	void set_model(shared_ptr<RecognizerModel> newModel) {
		if (m_isHighLoad) {
			unique_lock<shared_mutex> lock(m_modelMutex);
			m_model = newModel;
		}
		else {
			atomic_store(&m_model, newModel);
		}
	}
};
