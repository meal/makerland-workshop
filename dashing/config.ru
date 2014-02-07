require 'dashing'

configure do
  set :auth_token, 'a8f77ce1a8d6a40403a31564f2233bcd'

  helpers do
    def protected!
     # Put any authentication code you want in here.
     # This method is run before accessing any resource.
    end
  end
end

map Sinatra::Application.assets_prefix do
  run Sinatra::Application.sprockets
end

run Sinatra::Application