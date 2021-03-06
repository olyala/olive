#include "audiobackend.h"

#include "audioworker.h"

AudioBackend::AudioBackend(QObject *parent) :
  AudioRenderBackend(parent)
{
}

AudioBackend::~AudioBackend()
{
  Close();
}

QIODevice *AudioBackend::GetAudioPullDevice()
{
  pull_device_.setFileName(CachePathName());

  return &pull_device_;
}

bool AudioBackend::InitInternal()
{
  // Initiate one thread per CPU core
  for (int i=0;i<threads().size();i++) {
    // Create one processor object for each thread
    AudioWorker* processor = new AudioWorker();
    processor->SetParameters(params());
    processors_.append(processor);
  }

  return true;
}

void AudioBackend::CloseInternal()
{
}

bool AudioBackend::CompileInternal()
{
  // This backend doesn't compile anything yet
  return true;
}

void AudioBackend::DecompileInternal()
{
  // This backend doesn't compile anything yet
}

void AudioBackend::ConnectWorkerToThis(RenderWorker *worker)
{
  connect(worker, &RenderWorker::CompletedCache, this, &AudioBackend::ThreadCompletedCache);
}

void AudioBackend::ThreadCompletedCache(NodeDependency dep, NodeValueTable data, qint64 job_time)
{
  SetWorkerBusyState(static_cast<RenderWorker*>(sender()), false);

  if (job_time == render_job_info_.value(dep.range())) {
    render_job_info_.remove(dep.range());

    QByteArray cached_samples = data.Get(NodeParam::kSamples).toByteArray();

    int offset = params().time_to_bytes(dep.in());
    int length = params().time_to_bytes(dep.range().length());
    int out_point = offset + length;

    QFile f(CachePathName());
    if (f.open(QFile::WriteOnly | QFile::Append)) {

      if (f.size() < out_point) {
        f.resize(out_point);
      }

      f.seek(offset);

      // Replace data with this data
      int copy_length = qMin(length, cached_samples.size());

      f.write(cached_samples.data(), copy_length);

      if (copy_length < length) {
        // Fill in remainder with silence
        QByteArray empty_space(length - copy_length, 0);
        f.write(empty_space);
      }

      f.close();
    } else {
      qWarning() << "Failed to write to cached PCM file";
    }
  }

  CacheNext();
}
